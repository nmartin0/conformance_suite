/*
 * Without POSIX_SPAWN_RESETIDS the child inherits the caller's effective group ID: a caller .
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.posix_spawn.048
 */
#include "pc.h"
#include <errno.h>
#include <fcntl.h>
#include <limits.h>
#include <sched.h>
#include <signal.h>
#include <spawn.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/resource.h>
#include <sys/stat.h>
#include <sys/statvfs.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#ifndef PATH_MAX
#define PATH_MAX 4096
#endif

extern char **environ;

static char *self;		/* this program: the new process image */
static int rfd = -1;		/* in the image: where its report goes */
static int gargc;		/* in the image: its own argc and argv */
static char **gargv;
static int report[2] = { -1, -1 };
static int ctl[2] = { -1, -1 };
static char fdarg[16];
static pid_t kid = -1;
static pid_t reaped = -1;
static volatile sig_atomic_t timedout;
/* Four bytes that copy_self_tagged() rewrites, so that copies can be told apart. */
static volatile char tag[] = "PCSPAWN-TAG:ZZZZ";

static void image(const char *mode, int n, char **a);
static void parent(void);

/* In the image: adds to the report. */
static void say(const char *fmt, ...)
{
	char b[700];
	va_list ap;
	int n;

	va_start(ap, fmt);
	n = vsnprintf(b, sizeof b, fmt, ap);
	va_end(ap);
	if (n > 0)
		(void)write(rfd, b, (size_t)n < sizeof b ? (size_t)n : sizeof b - 1);
}

static void on_alarm(int sig)
{
	(void)sig;
	timedout = 1;
}

static void nothing(int sig)
{
	(void)sig;
}

/* Setup: dispositions the test needs, and a bound on every wait. */
static void init(int argc, char **argv, const char *id)
{
	struct sigaction sa;
	sigset_t s;

	pc_begin(id);
	pc_check(argc >= 1 && argv[0][0] == '/', "argv[0] is not an absolute path");
	self = argv[0];
	memset(&sa, 0, sizeof sa);
	sigemptyset(&sa.sa_mask);
	sa.sa_handler = SIG_IGN;
	(void)sigaction(SIGPIPE, &sa, NULL);
	sa.sa_handler = SIG_DFL;
	(void)sigaction(SIGCHLD, &sa, NULL);
	sa.sa_handler = on_alarm;
	(void)sigaction(SIGALRM, &sa, NULL);
	sigemptyset(&s);
	sigaddset(&s, SIGALRM);
	sigaddset(&s, SIGCHLD);
	(void)sigprocmask(SIG_UNBLOCK, &s, NULL);
	(void)alarm(40);
}

static void must(int rc, const char *what)
{
	if (rc != 0)
		pc_unresolved("%s failed (%s)", what, pc_errname(rc));
}

static void check_rc(int rc, const char *what)
{
	if (rc != 0)
		pc_fail("%s returned %s (%d), expected 0", what, pc_errname(rc), rc);
}

static void mkreport(void)
{
	if (report[0] != -1)
		(void)close(report[0]);
	if (report[1] != -1)
		(void)close(report[1]);
	if (pipe(report) != 0)
		pc_unresolved("pipe failed");
	(void)snprintf(fdarg, sizeof fdarg, "%d", report[1]);
}

/* The parent's copy of the report pipe's write end, once the child exists. */
static void closew(void)
{
	if (report[1] != -1)
		(void)close(report[1]);
	report[1] = -1;
}

static char *iv[48];

/* Arguments that run the image in mode m (argv[0] is argv0), then extra ones. */
static char **imgv(const char *argv0, const char *m, ...)
{
	va_list ap;
	const char *a;
	int n = 0;

	iv[n++] = (char *)argv0;
	iv[n++] = "img";
	iv[n++] = (char *)m;
	iv[n++] = fdarg;
	va_start(ap, m);
	while ((a = va_arg(ap, const char *)) != NULL && n < 46)
		iv[n++] = (char *)a;
	va_end(ap);
	iv[n] = NULL;
	return iv;
}

static void giveup(const char *why)
{
	if (kid > 0) {
		(void)kill(kid, SIGKILL);
		(void)waitpid(kid, NULL, 0);
	}
	pc_unresolved("%s (no answer within the time allowed)", why);
}

/* Reads fd *fdp to end of file into buf (bounded by the alarm) and closes it. */
static size_t readall(int *fdp, char *buf, size_t size)
{
	size_t n = 0;
	ssize_t r;

	while (n < size - 1) {
		r = read(*fdp, buf + n, size - 1 - n);
		if (r > 0) {
			n += (size_t)r;
			continue;
		}
		if (r < 0 && errno == EINTR) {
			if (timedout)
				giveup("a read");
			continue;
		}
		break;
	}
	buf[n] = '\0';
	(void)close(*fdp);
	*fdp = -1;
	return n;
}

static size_t collect(char *buf, size_t size)
{
	if (report[0] == -1) {
		buf[0] = '\0';
		return 0;
	}
	return readall(&report[0], buf, size);
}

/* Waits for the child (kid, or any child when kid is not positive). */
static int reap(void)
{
	int st;
	pid_t r;

	for (;;) {
		r = waitpid(kid > 0 ? kid : -1, &st, 0);
		if (r >= 0)
			break;
		if (errno == EINTR) {
			if (timedout)
				giveup("waitpid");
			continue;
		}
		pc_unresolved("waitpid failed (%s)", pc_errname(errno));
	}
	reaped = r;
	return st;
}

static int finish(char *buf, size_t size)
{
	(void)collect(buf, size);
	return reap();
}

/* The image reported "ok" and ended with status want. */
static void expect_ok(const char *buf, int st, int want)
{
	if (strcmp(buf, "ok") != 0) {
		if (buf[0] == '\0' && WIFEXITED(st) && WEXITSTATUS(st) == 127)
			pc_fail("the child ended with status 127: the spawn failed after the call returned");
		pc_fail("the new image reports: %s (status %#x)", buf[0] != '\0' ? buf : "nothing", st);
	}
	pc_check(WIFEXITED(st) && WEXITSTATUS(st) == want, "the new image ended with status %#x, expected exit %d", st, want);
}

/*
 * After a spawn call that must fail returned rc: either one of the error
 * numbers in allowed (ended by 0) and no child exists, or 0 with the child
 * ending with status 127 without having run the image (the image says "ran").
 */
static void expect_failure(const char *what, int rc, const int *allowed)
{
	char buf[128];
	int st, i, ok = 0;
	pid_t w;

	closew();
	if (rc == 0) {
		size_t n = collect(buf, sizeof buf);

		if (kid > 0)
			st = reap();
		else
			st = reap();
		if (n != 0)
			pc_fail("%s returned 0 and the new image ran (%s)", what, buf);
		pc_check(WIFEXITED(st) && WEXITSTATUS(st) == 127,
		    "%s returned 0, but the child ended with status %#x, not 127", what, st);
		return;
	}
	for (i = 0; allowed[i] != 0; i++)
		if (rc == allowed[i])
			ok = 1;
	if (!ok)
		pc_fail("%s returned %s (%d), expected %s%s", what, pc_errname(rc), rc,
		    pc_errname(allowed[0]), allowed[1] != 0 ? " (or another listed error)" : "");
	do
		w = waitpid(-1, &st, WNOHANG);
	while (w == -1 && errno == EINTR);
	pc_check(w == -1 && errno == ECHILD, "%s failed with %s, but a child process exists", what, pc_errname(rc));
	if (report[0] != -1) {
		(void)close(report[0]);
		report[0] = -1;
	}
}

/* One spawn that must fail; p selects posix_spawnp(). */
static void try_fail(const char *what, int p, const char *path, const posix_spawn_file_actions_t *fa,
    const posix_spawnattr_t *at, const int *allowed)
{
	int rc;

	mkreport();
	kid = -1;
	if (p)
		rc = posix_spawnp(&kid, path, fa, at, imgv(self, "ran", (char *)NULL), environ);
	else
		rc = posix_spawn(&kid, path, fa, at, imgv(self, "ran", (char *)NULL), environ);
	expect_failure(what, rc, allowed);
}

static char *dupstr(const char *s)
{
	char *r = malloc(strlen(s) + 1);

	if (r == NULL)
		pc_unresolved("out of memory");
	return strcpy(r, s);
}

static char *fmt(const char *f, ...)
{
	char b[PATH_MAX + 64];
	va_list ap;

	va_start(ap, f);
	(void)vsnprintf(b, sizeof b, f, ap);
	va_end(ap);
	return dupstr(b);
}

/* Absolute path of rel in the current directory. */
static char *apath(const char *rel)
{
	char cwd[PATH_MAX];

	if (getcwd(cwd, sizeof cwd) == NULL)
		pc_unresolved("getcwd failed");
	return fmt("%s/%s", cwd, rel);
}

static char *identity(const char *path)
{
	struct stat sb;

	if (stat(path, &sb) != 0)
		pc_unresolved("stat of %s failed", path);
	return fmt("@%llu:%llu", (unsigned long long)sb.st_dev, (unsigned long long)sb.st_ino);
}

/* In the image: does path have the identity given as "@dev:ino"? */
static int has_identity(const char *path, const char *id)
{
	struct stat sb;
	char b[80];

	if (stat(path, &sb) != 0)
		return 0;
	(void)snprintf(b, sizeof b, "@%llu:%llu", (unsigned long long)sb.st_dev, (unsigned long long)sb.st_ino);
	return strcmp(b, id) == 0;
}

/* A free descriptor number at or above base. */
static int freefd(int base)
{
	int t = open("/dev/null", O_RDONLY), n;

	if (t == -1)
		pc_unresolved("cannot open /dev/null");
	n = fcntl(t, F_DUPFD, base);
	if (n == -1)
		pc_unresolved("cannot find a free descriptor");
	(void)close(t);
	(void)close(n);
	return n;
}

/* Opens path and moves the descriptor to a number at or above base. */
static int openhigh(const char *path, int flags, int base)
{
	int t = open(path, flags), n;

	if (t == -1)
		pc_unresolved("cannot open %s", path);
	n = fcntl(t, F_DUPFD, base);
	if (n == -1)
		pc_unresolved("cannot move a descriptor");
	(void)close(t);
	return n;
}

/* Spec strings for the image's "chk" mode. */
static char *fds(int fd, char k)
{
	return fmt("%d:%c", fd, k);
}

static char *rd(int fd, const char *text)
{
	return fmt("%d=%s", fd, text);
}

static char *num(long v)
{
	return fmt("%ld", v);
}

static char *kv(int sig, const char *state)
{
	return fmt("%d=%s", sig, state);
}

static void mkset(sigset_t *s, ...)
{
	va_list ap;
	int g;

	sigemptyset(s);
	va_start(ap, s);
	while ((g = va_arg(ap, int)) != 0)
		sigaddset(s, g);
	va_end(ap);
}

/* how: 0 default, 1 ignored, 2 caught. */
static void setdisp(int sig, int how)
{
	struct sigaction sa;

	memset(&sa, 0, sizeof sa);
	sigemptyset(&sa.sa_mask);
	sa.sa_handler = how == 0 ? SIG_DFL : how == 1 ? SIG_IGN : nothing;
	if (sigaction(sig, &sa, NULL) != 0)
		pc_unresolved("sigaction failed");
}

/* In the image: how signal sig is handled. */
static const char *dispname(int sig)
{
	struct sigaction sa;

	if (sigaction(sig, NULL, &sa) != 0)
		return "err";
	if (sa.sa_flags & SA_SIGINFO)
		return "caught";
	if (sa.sa_handler == SIG_DFL)
		return "dfl";
	if (sa.sa_handler == SIG_IGN)
		return "ign";
	return "caught";
}

/* In the image: reads fd to end of file (at most n-1 bytes) into b. */
static void slurp(int fd, char *b, size_t n)
{
	size_t k = 0;
	ssize_t r;

	while (k < n - 1 && (r = read(fd, b + k, n - 1 - k)) > 0)
		k += (size_t)r;
	b[k] = '\0';
}

/*
 * In the image: checks the specs in a. "N:o" descriptor N is open with
 * FD_CLOEXEC clear; "N:c" it is closed; "N=text" it reads exactly text
 * (and is open with FD_CLOEXEC clear); "@dev:ino" the working directory
 * is that directory.
 */
static void check_specs(int n, char **a)
{
	int i;
	char b[256];

	for (i = 0; i < n; i++) {
		if (a[i][0] == '@') {
			if (!has_identity(".", a[i])) {
				say("the working directory is not the expected one");
				return;
			}
			continue;
		} else {
			int fd = atoi(a[i]);
			char *eq = strchr(a[i], '=');
			int fl = fcntl(fd, F_GETFD);

			if (eq == NULL) {
				char k = *(strchr(a[i], ':') + 1);

				if (k == 'c' && fl != -1) {
					say("descriptor %d is open, expected closed", fd);
					return;
				}
				if (k == 'o' && fl == -1) {
					say("descriptor %d is closed, expected open", fd);
					return;
				}
				if (k == 'o' && (fl & FD_CLOEXEC)) {
					say("descriptor %d has FD_CLOEXEC set", fd);
					return;
				}
			} else {
				if (fl == -1) {
					say("descriptor %d is closed, expected open", fd);
					return;
				}
				if (fl & FD_CLOEXEC) {
					say("descriptor %d has FD_CLOEXEC set", fd);
					return;
				}
				slurp(fd, b, sizeof b);
				if (strcmp(b, eq + 1) != 0) {
					say("descriptor %d reads \"%s\", expected \"%s\"", fd, b, eq + 1);
					return;
				}
			}
		}
	}
	say("ok");
}

/* In the image: checks "sig=dfl|ign|caught" specs. */
static void check_disp(int n, char **a)
{
	int i;

	for (i = 0; i < n; i++) {
		int sig = atoi(a[i]);
		const char *want = strchr(a[i], '=') + 1;
		const char *got = dispname(sig);

		if (strcmp(got, want) != 0) {
			say("signal %d is %s, expected %s", sig, got, want);
			return;
		}
	}
	say("ok");
}

/* In the image: checks that exactly the signals numbered in a are blocked, among those listed here. */
static void check_mask(int n, char **a)
{
	static const int sigs[] = { SIGHUP, SIGINT, SIGQUIT, SIGUSR1, SIGUSR2, SIGTERM, SIGALRM, SIGCHLD, SIGPIPE };
	sigset_t cur;
	size_t i;
	int j;

	if (sigprocmask(SIG_BLOCK, NULL, &cur) != 0) {
		say("sigprocmask failed");
		return;
	}
	for (i = 0; i < sizeof sigs / sizeof sigs[0]; i++) {
		int want = 0, got = sigismember(&cur, sigs[i]);

		for (j = 0; j < n; j++)
			if (atoi(a[j]) == sigs[i])
				want = 1;
		if (got != want) {
			say("signal %d is %s", sigs[i], got ? "blocked" : "not blocked");
			return;
		}
	}
	say("ok");
}

/* Copies this program to path with the given mode, rewriting its tag to t4. */
static void copy_self_tagged(const char *path, mode_t mode, const char *t4)
{
	struct stat sb;
	char *b, pat[16];
	size_t len, off, i;
	ssize_t r;
	int in, out, hits = 0;

	in = open(self, O_RDONLY);
	if (in == -1 || fstat(in, &sb) != 0)
		pc_unresolved("cannot read %s", self);
	len = (size_t)sb.st_size;
	b = malloc(len + 1);
	if (b == NULL)
		pc_unresolved("out of memory");
	for (off = 0; off < len; off += (size_t)r) {
		r = read(in, b + off, len - off);
		if (r <= 0)
			pc_unresolved("short read of %s", self);
	}
	(void)close(in);
	/* The tag as this program holds it (a use that keeps the array in the executable). */
	for (i = 0; i < 16; i++)
		pat[i] = tag[i];
	for (i = 0; i + 16 <= len; i++)
		if (memcmp(b + i, pat, 16) == 0) {
			memcpy(b + i + 12, t4, 4);
			hits++;
			i += 15;
		}
	if (hits == 0)
		pc_unresolved("the tag was not found in %s", self);
	out = open(path, O_WRONLY | O_CREAT | O_TRUNC, 0700);
	if (out == -1)
		pc_unresolved("cannot create %s", path);
	for (off = 0; off < len; off += (size_t)r) {
		r = write(out, b + off, len - off);
		if (r <= 0)
			pc_unresolved("short write to %s", path);
	}
	if (close(out) != 0 || chmod(path, mode) != 0)
		pc_unresolved("cannot finish %s", path);
	free(b);
}

static void copy_self(const char *path, mode_t mode)
{
	copy_self_tagged(path, mode, "ZZZZ");
}

/* In the image: the four tag bytes. */
static void say_tag(void)
{
	say("%c%c%c%c", tag[12], tag[13], tag[14], tag[15]);
}

/* A helper process: reports the spawn result through ctl and ends. */
_Noreturn static void helper_end(int rc)
{
	char b[24];
	int n = snprintf(b, sizeof b, "%d", rc);

	if (n > 0)
		(void)write(ctl[1], b, (size_t)n);
	_exit(0);
}

/* Starts the helper; returns 1 in it. report must exist. */
static int start_helper(pid_t *h)
{
	if (pipe(ctl) != 0)
		pc_unresolved("pipe failed");
	*h = fork();
	if (*h == -1)
		pc_unresolved("fork failed");
	if (*h == 0) {
		(void)close(ctl[0]);
		(void)close(report[0]);
		return 1;
	}
	(void)close(ctl[1]);
	ctl[1] = -1;
	return 0;
}

/* In the parent: the helper's result (the spawn return value; negative when the helper's own setup failed). */
static int helper_result(pid_t h)
{
	char b[32];
	size_t n = readall(&ctl[0], b, sizeof b);
	int st;

	while (waitpid(h, &st, 0) == -1) {
		if (errno == EINTR && !timedout)
			continue;
		pc_unresolved("waitpid of the helper failed");
	}
	if (n == 0)
		pc_unresolved("the helper process failed");
	return atoi(b);
}


static void image(const char *mode, int n, char **a)
{
	(void)mode;
	(void)n;
	(void)a;
	say("%ld %ld %ld %ld", (long)getuid(), (long)geteuid(), (long)getgid(), (long)getegid());
}

static void parent(void)
{
	posix_spawnattr_t at;
	char buf[256];
	long ru, eu, rg, eg;
	int rc, st;
	char *prog;
	pid_t h;

	if (getuid() != 0 || geteuid() != 0 || getgid() != 0 || getegid() != 0)
		pc_unresolved("needs real and effective user and group IDs 0");
	pc_check(chmod(".", 0711) == 0, "chmod failed");
	copy_self("prog", 0755);
	prog = apath("prog");
	must(posix_spawnattr_init(&at), "posix_spawnattr_init");
	
	mkreport();
	pc_test();
	if (start_helper(&h)) {
		if (setegid(65534) != 0)
			helper_end(-1);
		rc = posix_spawn(&kid, prog, NULL, &at, imgv("prog", "ids", (char *)NULL), environ);
		closew();
		if (rc == 0 && waitpid(kid, &st, 0) == kid && WIFEXITED(st) && WEXITSTATUS(st) == 127)
			helper_end(-2);
		helper_end(rc);
	}
	closew();
	(void)collect(buf, sizeof buf);
	rc = helper_result(h);
	if (rc == -2)
		pc_unresolved("the child ended with status 127: the image could not be started with these IDs");
	if (rc < 0)
		pc_unresolved("the helper could not change its IDs");
	if (rc != 0)
		pc_unresolved("posix_spawn returned %s: the image could not be started with these IDs", pc_errname(rc));
	if (sscanf(buf, "%ld %ld %ld %ld", &ru, &eu, &rg, &eg) != 4)
		pc_fail("the image reports \"%s\"", buf);
	pc_check(rg == 0 && eg == 65534, "the image has real group ID %ld and effective group ID %ld, expected 0 and 65534", rg, eg);
}

int main(int argc, char **argv)
{
	if (argc >= 4 && strcmp(argv[1], "img") == 0) {
		rfd = atoi(argv[3]);
		gargc = argc;
		gargv = argv;
		image(argv[2], argc - 4, argv + 4);
		return 0;
	}
	init(argc, argv, "XSH.posix_spawn.048");
	parent();
	pc_pass();
}
