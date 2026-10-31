/*
 * pcexec - run one posixcheck test under controlled conditions.
 *
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * usage: pcexec -w BASE -o STATUS [-t SECS] [-l NAME=VAL[,...]] [-u USER]
 *               [-e NAME=VALUE]... -- PROGRAM [ARG...]
 *
 *   -w BASE   directory in which to create the test's private work dir
 *   -o STATUS file to receive the outcome (see below)
 *   -t SECS   timeout; the whole process group is killed when it expires
 *   -l ...    resource limits: as, core, cpu, data, fsize, nofile, stack
 *   -u USER   run as USER (requires root)
 *   -e N=V    add N=V to the test's environment
 *
 * The test runs in its own process group, without a controlling terminal
 * (in a session that a helper child of ours leads), in a fresh private
 * directory (removed afterwards), with standard input from /dev/null,
 * umask 022 and
 * a clean environment: PATH from confstr(_CS_PATH), HOME and TMPDIR set to
 * the private directory, LC_ALL=POSIX, every PC_* variable from our own
 * environment, plus any -e additions.
 *
 * STATUS receives one line:
 *     EXIT <status> <ms>      the test exited
 *     SIGNAL <signo> <ms>     the test was killed by a signal
 *     TIMEOUT <ms>            the timeout expired
 *     ERROR <message>         pcexec could not run the test
 * followed by "CLEANUP ok" or "CLEANUP failed".
 *
 * pcexec is a harness tool, not a test, so it is compiled without strict
 * feature-test macros: it needs setgroups(), which POSIX does not define.
 */
#include <sys/types.h>
#include <sys/resource.h>
#include <sys/stat.h>
#include <sys/wait.h>

#include <dirent.h>
#include <errno.h>
#include <fcntl.h>
#include <grp.h>
#include <limits.h>
#include <pwd.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

extern char **environ;

#define MAXENV 256

static const char *status_path;
static volatile sig_atomic_t timed_out;

static void on_alarm(int sig)
{
	(void)sig;
	timed_out = 1;
}

static void write_status(const char *line, int cleanup_ok)
{
	FILE *f = fopen(status_path, "w");
	if (f == NULL) {
		fprintf(stderr, "pcexec: cannot write %s: %s\n", status_path,
		    strerror(errno));
		exit(3);
	}
	fprintf(f, "%s\nCLEANUP %s\n", line, cleanup_ok ? "ok" : "failed");
	if (fclose(f) != 0)
		exit(3);
}

static void die(const char *msg)
{
	char line[512];
	(void)snprintf(line, sizeof line, "ERROR %s", msg);
	if (status_path)
		write_status(line, 1);
	fprintf(stderr, "pcexec: %s\n", msg);
	exit(3);
}

static void usage(void)
{
	fprintf(stderr, "usage: pcexec -w BASE -o STATUS [-t SECS] "
	    "[-l NAME=VAL,...] [-u USER] [-e N=V]... -- PROG [ARG...]\n");
	exit(2);
}

static long now_ms(void)
{
	struct timespec ts;
#ifdef CLOCK_MONOTONIC
	if (clock_gettime(CLOCK_MONOTONIC, &ts) == 0)
		return (long)ts.tv_sec * 1000L + ts.tv_nsec / 1000000L;
#endif
	(void)clock_gettime(CLOCK_REALTIME, &ts);
	return (long)ts.tv_sec * 1000L + ts.tv_nsec / 1000000L;
}

/* ---------- resource limits ---------- */

struct limit {
	const char *name;
	int resource;
};

static const struct limit limits[] = {
#ifdef RLIMIT_AS
	{ "as", RLIMIT_AS },
#else
	{ "as", RLIMIT_DATA },	/* no address-space limit here: bound the data segment */
#endif
	{ "core", RLIMIT_CORE },
	{ "cpu", RLIMIT_CPU },
	{ "data", RLIMIT_DATA },
	{ "fsize", RLIMIT_FSIZE },
	{ "nofile", RLIMIT_NOFILE },
	{ "stack", RLIMIT_STACK },
};

/* Runs in the child, so errors are reported and the child exits 126. */
static void child_die(const char *msg)
{
	fprintf(stderr, "pcexec: %s\n", msg);
	_exit(126);
}

static rlim_t parse_size(const char *s)
{
	char *end;
	unsigned long long v;

	errno = 0;
	v = strtoull(s, &end, 10);
	if (errno != 0 || end == s)
		child_die("bad limit value");
	switch (*end) {
	case '\0': break;
	case 'K': v *= 1024ULL; break;
	case 'M': v *= 1024ULL * 1024ULL; break;
	case 'G': v *= 1024ULL * 1024ULL * 1024ULL; break;
	default: child_die("bad limit suffix");
	}
	return (rlim_t)v;
}

/* Runs in the child. "as=256M,nofile=64" */
static void apply_limits(char *spec)
{
	char *item = spec;

	while (item && *item) {
		char *next = strchr(item, ',');
		char *eq;
		size_t i;
		struct rlimit rl;

		if (next)
			*next++ = '\0';
		eq = strchr(item, '=');
		if (eq == NULL)
			child_die("bad -l item");
		*eq = '\0';
		for (i = 0; i < sizeof limits / sizeof limits[0]; i++)
			if (strcmp(limits[i].name, item) == 0)
				break;
		if (i == sizeof limits / sizeof limits[0])
			child_die("unknown limit name");
		rl.rlim_cur = rl.rlim_max = parse_size(eq + 1);
		if (setrlimit(limits[i].resource, &rl) != 0)
			child_die("setrlimit failed");
		item = next;
	}
}

/* ---------- private directory ---------- */

/* Remove path recursively, without following symbolic links. */
static int remove_tree(const char *path)
{
	struct stat st;
	DIR *d;
	struct dirent *de;
	int ok = 1;

	if (lstat(path, &st) != 0)
		return errno == ENOENT;
	if (!S_ISDIR(st.st_mode))
		return unlink(path) == 0;
	(void)chmod(path, 0700);
	d = opendir(path);
	if (d == NULL)
		return 0;
	while ((de = readdir(d)) != NULL) {
		char sub[PATH_MAX];
		if (strcmp(de->d_name, ".") == 0 ||
		    strcmp(de->d_name, "..") == 0)
			continue;
		if (snprintf(sub, sizeof sub, "%s/%s", path, de->d_name) >=
		    (int)sizeof sub) {
			ok = 0;
			continue;
		}
		if (!remove_tree(sub))
			ok = 0;
	}
	(void)closedir(d);
	if (rmdir(path) != 0)
		ok = 0;
	return ok;
}

/* ---------- main ---------- */

int main(int argc, char **argv)
{
	const char *base = NULL, *user = NULL;
	char *limitspec = NULL;
	char *extra[MAXENV];
	int nextra = 0;
	long timeout = 60;
	char work[PATH_MAX];
	char *env[MAXENV + 16];
	char envbuf[4][PATH_MAX + 32];
	int nenv = 0, i, c, status, cleanup_ok;
	struct sigaction sa;
	struct passwd *pw = NULL;
	pid_t pid, gpid = -1;
	int pfd[2], helper_ok;
	long t0, ms;
	char line[256];

	while ((c = getopt(argc, argv, "w:o:t:l:u:e:")) != -1) {
		switch (c) {
		case 'w': base = optarg; break;
		case 'o': status_path = optarg; break;
		case 't': timeout = strtol(optarg, NULL, 10); break;
		case 'l': limitspec = optarg; break;
		case 'u': user = optarg; break;
		case 'e':
			if (nextra >= MAXENV || strchr(optarg, '=') == NULL)
				usage();
			extra[nextra++] = optarg;
			break;
		default: usage();
		}
	}
	argv += optind;
	argc -= optind;
	if (base == NULL || status_path == NULL || argc < 1 || timeout < 1)
		usage();

	if (user) {
		pw = getpwnam(user);
		if (pw == NULL)
			die("unknown user for -u");
	}

	if (snprintf(work, sizeof work, "%s/pc.XXXXXX", base) >=
	    (int)sizeof work)
		die("work path too long");
	if (mkdtemp(work) == NULL)
		die("mkdtemp failed");
	if (pw && chown(work, pw->pw_uid, pw->pw_gid) != 0)
		die("chown of work dir failed");

	/* Environment: PATH, HOME, TMPDIR, LC_ALL, PC_*, then -e. */
	{
		size_t n = confstr(_CS_PATH, NULL, 0);
		char cspath[PATH_MAX];
		if (n == 0 || n > sizeof cspath)
			die("confstr(_CS_PATH) unavailable");
		(void)confstr(_CS_PATH, cspath, sizeof cspath);
		(void)snprintf(envbuf[0], sizeof envbuf[0], "PATH=%s", cspath);
		(void)snprintf(envbuf[1], sizeof envbuf[1], "HOME=%s", work);
		(void)snprintf(envbuf[2], sizeof envbuf[2], "TMPDIR=%s", work);
		(void)snprintf(envbuf[3], sizeof envbuf[3], "LC_ALL=POSIX");
		for (i = 0; i < 4; i++)
			env[nenv++] = envbuf[i];
	}
	for (i = 0; environ[i] && nenv < MAXENV; i++)
		if (strncmp(environ[i], "PC_", 3) == 0)
			env[nenv++] = environ[i];
	for (i = 0; i < nextra && nenv < MAXENV + 15; i++)
		env[nenv++] = extra[i];
	env[nenv] = NULL;

	memset(&sa, 0, sizeof sa);
	sa.sa_handler = on_alarm;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = 0;	/* no SA_RESTART: waitpid must be interrupted */
	if (sigaction(SIGALRM, &sa, NULL) != 0)
		die("sigaction failed");

	if (pipe(pfd) != 0)
		die("pipe failed");
	t0 = now_ms();
	pid = fork();
	if (pid < 0)
		die("fork failed");
	if (pid == 0) {
		/*
		 * The helper: it leads a new session with no controlling
		 * terminal and runs the test in a child of its own. Were the
		 * test left on the terminal the driver was started from, it
		 * would be in a background process group of that terminal, and
		 * an interactive shell started there stops itself. The test is
		 * in a process group of its own whose parent is in another
		 * group of the same session, as before: it is neither a session
		 * leader nor in an orphaned process group. The helper sends the
		 * test's process ID and then its wait status over the pipe.
		 */
		pid_t g;
		int st;
		long v;

		(void)close(pfd[0]);
		(void)setsid();
		g = fork();
		if (g < 0)
			_exit(125);
		if (g > 0) {
			(void)setpgid(g, g);	/* close the race with the test */
			v = (long)g;
			if (write(pfd[1], &v, sizeof v) != (ssize_t)sizeof v)
				_exit(125);
			while (waitpid(g, &st, 0) == -1)
				if (errno != EINTR)
					_exit(125);
			if (write(pfd[1], &st, sizeof st) != (ssize_t)sizeof st)
				_exit(125);
			_exit(0);
		}
	}
	if (pid == 0) {
		int fd;
		(void)close(pfd[1]);
		(void)setpgid(0, 0);
		sa.sa_handler = SIG_DFL;
		(void)sigaction(SIGALRM, &sa, NULL);
		{
			/*
			 * Tests start with no signal blocked and every signal at
			 * its default action: an ignored signal would otherwise
			 * be inherited across exec from whoever ran the driver.
			 */
			sigset_t none;
			int s;
			sa.sa_handler = SIG_DFL;
			for (s = 1; s < 128; s++)
				(void)sigaction(s, &sa, NULL);	/* fails harmlessly past the last */
			sigemptyset(&none);
			(void)sigprocmask(SIG_SETMASK, &none, NULL);
		}
		if (chdir(work) != 0)
			_exit(126);
		(void)umask(022);
		fd = open("/dev/null", O_RDONLY);
		if (fd >= 0 && fd != 0) {
			(void)dup2(fd, 0);
			(void)close(fd);
		}
		/*
		 * Tests start with only 0, 1 and 2 open: descriptors inherited
		 * from the driver would distort descriptor-allocation and
		 * limit tests.
		 */
		{
			long max = sysconf(_SC_OPEN_MAX);
			if (max < 0 || max > 65536)
				max = 65536;
			for (fd = 3; fd < max; fd++)
				(void)close(fd);
		}
		if (limitspec)
			apply_limits(limitspec);
		if (pw) {
			if (setgroups(1, &pw->pw_gid) != 0 ||
			    setgid(pw->pw_gid) != 0 ||
			    setuid(pw->pw_uid) != 0) {
				fprintf(stderr, "pcexec: cannot become %s\n",
				    user);
				_exit(126);
			}
		}
		execve(argv[0], argv, env);
		fprintf(stderr, "pcexec: cannot execute %s: %s\n", argv[0],
		    strerror(errno));
		_exit(127);
	}
	(void)close(pfd[1]);
	{
		long v;
		if (read(pfd[0], &v, sizeof v) == (ssize_t)sizeof v && v > 1)
			gpid = (pid_t)v;
	}
	alarm((unsigned)timeout);

	for (;;) {
		pid_t r = waitpid(pid, &status, 0);
		if (r == pid)
			break;
		if (r == -1 && errno == EINTR) {
			if (timed_out) {
				if (gpid > 1)
					(void)kill(-gpid, SIGKILL);
				(void)kill(-pid, SIGKILL);
			}
			continue;
		}
		die("waitpid failed");
	}
	alarm(0);
	ms = now_ms() - t0;
	/* Anything the test left running in its group goes too. */
	if (gpid > 1)
		(void)kill(-gpid, SIGKILL);
	(void)kill(-pid, SIGKILL);
	/* The test's own status, which the helper sends when it ends. */
	helper_ok = 1;
	if (!timed_out) {
		int st;
		if (gpid > 1 && WIFEXITED(status) && WEXITSTATUS(status) == 0 &&
		    read(pfd[0], &st, sizeof st) == (ssize_t)sizeof st)
			status = st;
		else
			helper_ok = 0;
	}
	(void)close(pfd[0]);

	if (!helper_ok)
		(void)snprintf(line, sizeof line, "ERROR the helper that runs the test failed");
	else if (timed_out)
		(void)snprintf(line, sizeof line, "TIMEOUT %ld", ms);
	else if (WIFEXITED(status))
		(void)snprintf(line, sizeof line, "EXIT %d %ld",
		    WEXITSTATUS(status), ms);
	else if (WIFSIGNALED(status))
		(void)snprintf(line, sizeof line, "SIGNAL %d %ld",
		    WTERMSIG(status), ms);
	else
		(void)snprintf(line, sizeof line, "ERROR unexpected wait status");

	cleanup_ok = remove_tree(work);
	write_status(line, cleanup_ok);
	return 0;
}
