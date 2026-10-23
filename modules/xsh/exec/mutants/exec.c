/*
 * exec.c - planted faults in the exec family, for mutation checks (CI only).
 *
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * A preload library; faults are named "exec.<fault>" and apply only while
 * PC_PHASE is "test". glibc's execv(), execl() and execvp() call execve()
 * internally, past any preload, so every member of the family is wrapped
 * here. With no fault planted each wrapper calls the C library's own
 * function; with one planted, they are built on the real execve() (and
 * execvp() does its own PATH search), so that the fault can sit anywhere.
 */
#define _GNU_SOURCE 1
#include <dlfcn.h>
#include <errno.h>
#include <fcntl.h>
#include <signal.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include <sys/resource.h>
#include <sys/stat.h>
#include <time.h>
#include <grp.h>
#include <stdio.h>
#include <sys/wait.h>
#include <unistd.h>

extern char **environ;

typedef int (*execve_fn)(const char *, char *const[], char *const[]);
typedef int (*execv_fn)(const char *, char *const[]);
typedef int (*fexecve_fn)(int, char *const[], char *const[]);

#define MAXARGS 4096

static char **start_env;	/* the environment as the program started */

__attribute__((constructor)) static void save_env(void)
{
	size_t n = 0, i;

	while (environ != NULL && environ[n] != NULL)
		n++;
	start_env = malloc((n + 1) * sizeof *start_env);
	if (start_env == NULL)
		return;
	for (i = 0; i < n; i++)
		start_env[i] = environ[i];
	start_env[n] = NULL;
}

static char phase[16];	/* the last PC_PHASE set, in case environ is replaced */

/* pc.c sets PC_PHASE with setenv(); remember it. */
int setenv(const char *name, const char *value, int overwrite)
{
	static int (*real)(const char *, const char *, int);

	if (real == NULL)
		real = (int (*)(const char *, const char *, int))dlsym(RTLD_NEXT, "setenv");
	if (strcmp(name, "PC_PHASE") == 0 && strlen(value) < sizeof phase)
		strcpy(phase, value);
	return real(name, value, overwrite);
}

/* getenv(), or the environment the program started with. */
static const char *lookup(const char *name)
{
	const char *v = getenv(name);
	size_t i, n = strlen(name);

	for (i = 0; v == NULL && start_env != NULL && start_env[i] != NULL; i++)
		if (strncmp(start_env[i], name, n) == 0 && start_env[i][n] == '=')
			v = start_env[i] + n + 1;
	return v;
}

/*
 * Faults apply only in the test process (and its children), where pc.c set
 * the phase; never in a new image or a shell it runs, which inherit
 * PC_PHASE but not this memory.
 */
static const char *mutant(void)
{
	const char *ph = phase[0] != '\0' ? phase : NULL;
	const char *m = lookup("PC_MUTANT");
	if (ph == NULL || strcmp(ph, "test") != 0 || m == NULL ||
	    strncmp(m, "exec.", 5) != 0)
		return NULL;
	return m + 5;
}

#define IS(name) (m != NULL && strcmp(m, name) == 0)

static execve_fn real_execve(void)
{
	static execve_fn f;
	if (f == NULL)
		f = (execve_fn)dlsym(RTLD_NEXT, "execve");
	return f;
}

/* Process-state faults, done just before the real call. */
static void before(const char *m)
{
	int fd, sig;
	struct sigaction sa;

	if (IS("flushes_streams"))
		(void)fflush(NULL);	/* writes the old image's buffered data */

	for (fd = 3; fd < 1024; fd++) {
		int fl = fcntl(fd, F_GETFD);
		if (fl == -1)
			continue;
		if (IS("cloexec_ignored") && (fl & FD_CLOEXEC))
			(void)fcntl(fd, F_SETFD, fl & ~FD_CLOEXEC);
		if (IS("fds_closed") && !(fl & FD_CLOEXEC)) {
			struct stat st;
			/* regular files only: the report pipe must stay */
			if (fstat(fd, &st) == 0 && S_ISREG(st.st_mode))
				(void)close(fd);
		}
#ifdef FD_CLOFORK
		if (IS("clofork_cleared") && (fl & FD_CLOFORK))
			(void)fcntl(fd, F_SETFD, fl & ~FD_CLOFORK);
#endif
	}
	for (sig = 1; sig < 65; sig++) {
		if (sig == SIGKILL || sig == SIGSTOP || sigaction(sig, NULL, &sa) != 0)
			continue;
		if (IS("ign_reset") && sa.sa_handler == SIG_IGN && sig != SIGCHLD) {
			sa.sa_handler = SIG_DFL;
			(void)sigaction(sig, &sa, NULL);
		}
		if (IS("dfl_ignored") && sa.sa_handler == SIG_DFL &&
		    (sig == SIGUSR1 || sig == SIGUSR2 || sig == SIGTERM || sig == SIGINT ||
		     sig == SIGHUP || sig == SIGPIPE)) {
			sa.sa_handler = SIG_IGN;
			(void)sigaction(sig, &sa, NULL);
		}
		if (IS("caught_ignored") && sa.sa_handler != SIG_DFL && sa.sa_handler != SIG_IGN) {
			sa.sa_handler = SIG_IGN;
			sa.sa_flags &= ~SA_SIGINFO;
			(void)sigaction(sig, &sa, NULL);
		}
	}
	if (IS("mask_cleared")) {
		sigset_t none;
		sigemptyset(&none);
		(void)sigprocmask(SIG_SETMASK, &none, NULL);
	}
	if (IS("pending_dropped")) {
		sigset_t p, one;
		struct timespec zero = { 0, 0 };
		(void)sigpending(&p);
		for (sig = 1; sig < 65; sig++) {
			if (sigismember(&p, sig) == 1) {
				sigemptyset(&one);
				sigaddset(&one, sig);
				(void)sigtimedwait(&one, NULL, &zero);
			}
		}
	}
	if (IS("pgrp_changed"))
		(void)setpgid(0, 0);
	if (IS("nice_changed"))
		(void)nice(1);
	if (IS("groups_dropped") && (getgroups(0, NULL) <= 0 || setgroups(0, NULL) != 0))
		(void)fprintf(stderr, "pcmutant: cannot plant groups_dropped (no groups, or no privilege)\n");
	if (IS("alarm_cleared"))
		(void)alarm(0);
	if (IS("cwd_reset"))
		(void)chdir("/");
	if (IS("umask_reset"))
		(void)umask(022);
	if (IS("rlimit_raised")) {
		int res[2] = { RLIMIT_NOFILE, RLIMIT_FSIZE }, i;
		for (i = 0; i < 2; i++) {
			struct rlimit rl;
			if (getrlimit(res[i], &rl) == 0) {
				rl.rlim_cur = rl.rlim_max;
				(void)setrlimit(res[i], &rl);
			}
		}
	}
}

/* Error-number faults. */
static int map_errno(const char *m, int e)
{
	if (IS("enoent_as_eacces") && e == ENOENT)
		return EACCES;
	if (IS("eacces_as_enoent") && e == EACCES)
		return ENOENT;
	if (IS("enotdir_as_enoent") && e == ENOTDIR)
		return ENOENT;
	if (IS("enotdir_as_eacces") && e == ENOTDIR)
		return EACCES;
	if (IS("enametoolong_as_eacces") && e == ENAMETOOLONG)
		return EACCES;
	if (IS("eloop_as_enoent") && e == ELOOP)
		return ENOENT;
	if (IS("enametoolong_as_enoent") && e == ENAMETOOLONG)
		return ENOENT;
	if (IS("enoexec_as_eacces") && e == ENOEXEC)
		return EACCES;
	if (IS("e2big_as_enomem") && e == E2BIG)
		return ENOMEM;
	if (IS("ebadf_as_einval") && e == EBADF)
		return EINVAL;
	return e;
}

static int faulty_execve(const char *m, const char *path, char *const argv[], char *const envp[])
{
	static char *av[MAXARGS + 1], *ev[MAXARGS + 2];
	size_t n = 0, k = 0, i;
	int r, e;

	for (i = 0; argv[i] != NULL; i++) {
		if (IS("empty_arg_dropped") && argv[i][0] == '\0')
			continue;
		if (n == MAXARGS) {
			(void)fprintf(stderr, "pcmutant: cannot plant: more than %d arguments\n", MAXARGS);
			errno = E2BIG;
			return -1;
		}
		av[n++] = argv[i];
	}
	if (IS("drop_last_arg") && n > 1)
		n--;
	av[n] = NULL;
	for (i = 0; envp != NULL && envp[i] != NULL; i++) {
		if (k == MAXARGS) {
			(void)fprintf(stderr, "pcmutant: cannot plant: more than %d environment strings\n", MAXARGS);
			errno = E2BIG;
			return -1;
		}
		ev[k++] = envp[i];
	}
	if (IS("env_dropped") && k > 0)
		k--;
	if (IS("env_extra"))
		ev[k++] = "PC_EXTRA=1";
	ev[k] = NULL;
	before(m);
	if (IS("slash_ignored")) {
		static char trimmed[4096];
		size_t len = strlen(path);
		if (len < sizeof trimmed) {
			strcpy(trimmed, path);
			while (len > 1 && trimmed[len - 1] == '/')
				trimmed[--len] = '\0';
			path = trimmed;
		}
	}
	if (IS("mode_ignored")) {
		struct stat st;
		if (stat(path, &st) == 0 && S_ISREG(st.st_mode))
			(void)chmod(path, st.st_mode | S_IXUSR);
	}
	if (IS("forks")) {
		/* a fake exec: runs the image in a child and passes on its status */
		pid_t c = fork();
		if (c == 0) {
			(void)real_execve()(path, av, ev);
			_exit(127);
		}
		if (c > 0) {
			int st;
			if (waitpid(c, &st, 0) == c)
				_exit(WIFEXITED(st) ? WEXITSTATUS(st) : 128);
		}
	}
	r = real_execve()(path, av, ev);
	e = errno;
	if (IS("scribbles") && argv[0] != NULL)
		((char **)argv)[0] = "scribbled";
	errno = map_errno(m, e);
	return r;
}

static char **env_for(const char *m)
{
	return IS("environ_ignored") && start_env != NULL ? start_env : environ;
}

int execve(const char *path, char *const argv[], char *const envp[])
{
	const char *m = mutant();

	if (m == NULL)
		return real_execve()(path, argv, envp);
	return faulty_execve(m, path, argv, envp);
}

int execv(const char *path, char *const argv[])
{
	const char *m = mutant();

	if (m == NULL) {
		static execv_fn f;
		if (f == NULL)
			f = (execv_fn)dlsym(RTLD_NEXT, "execv");
		return f(path, argv);
	}
	return faulty_execve(m, path, argv, env_for(m));
}

/* Runs file with sh, as execvp() does for a file in an unknown format. */
static int run_sh(const char *m, const char *file, char *const argv[], char *const envp[])
{
	static char *av[MAXARGS + 3];
	size_t n = 0, i;

	av[n++] = "sh";
	av[n++] = (char *)file;
	for (i = 1; argv[0] != NULL && argv[i] != NULL; i++) {
		if (n == MAXARGS) {
			errno = E2BIG;
			return -1;
		}
		av[n++] = argv[i];
	}
	av[n] = NULL;
	return faulty_execve(m, "/bin/sh", av, envp);
}

static int try_one(const char *m, const char *path, char *const argv[], char *const envp[])
{
	int r = faulty_execve(m, path, argv, envp);

	if (r == -1 && errno == ENOEXEC && !IS("no_sh_fallback"))
		return run_sh(m, path, argv, envp);
	return r;
}

static int faulty_execvp(const char *m, const char *file, char *const argv[])
{
	char *const *envp = env_for(m);
	const char *path, *dirs[256];
	char buf[4096], copy[4096], *p;
	int nd = 0, i, seen_eacces = 0;

	if (*file == '\0') {
		errno = ENOENT;
		return -1;
	}
	if (strchr(file, '/') != NULL && !IS("slash_searched"))
		return try_one(m, file, argv, envp);
	path = getenv("PATH");
	if (path == NULL)
		path = "/usr/bin:/bin";
	if (strlen(path) >= sizeof copy) {
		errno = ENAMETOOLONG;
		return -1;
	}
	strcpy(copy, path);
	for (p = copy; nd < 256;) {
		char *c = strchr(p, ':');
		dirs[nd++] = *p == '\0' || p == c ? "." : p;
		if (c == NULL)
			break;
		*c = '\0';
		p = c + 1;
	}
	for (i = 0; i < nd; i++) {
		const char *d = dirs[IS("path_order_reversed") ? nd - 1 - i : i];
		if (strlen(d) + strlen(file) + 2 > sizeof buf)
			continue;
		strcpy(buf, d);
		strcat(buf, "/");
		strcat(buf, file);
		(void)try_one(m, buf, argv, envp);
		if (errno == EACCES)
			seen_eacces = 1;
		else if (errno != ENOENT && errno != ENOTDIR)
			return -1;
	}
	errno = seen_eacces ? EACCES : ENOENT;
	return -1;
}

int execvp(const char *file, char *const argv[])
{
	const char *m = mutant();

	if (m == NULL) {
		static execv_fn f;
		if (f == NULL)
			f = (execv_fn)dlsym(RTLD_NEXT, "execvp");
		return f(file, argv);
	}
	return faulty_execvp(m, file, argv);
}

/* Collects a variadic argument list ending in a null pointer. */
static char **collect(const char *arg0, va_list *ap)
{
	static char *av[MAXARGS + 1];
	size_t n = 0;
	char *a = (char *)arg0;

	while (a != NULL) {
		if (n == MAXARGS)
			return NULL;	/* the caller fails with E2BIG */
		av[n++] = a;
		a = va_arg(*ap, char *);
	}
	av[n] = NULL;
	return av;
}

int execl(const char *path, const char *arg0, ...)
{
	va_list ap;
	char **av;

	va_start(ap, arg0);
	av = collect(arg0, &ap);
	va_end(ap);
	if (av == NULL) {
		errno = E2BIG;
		return -1;
	}
	return execv(path, av);
}

int execle(const char *path, const char *arg0, ...)
{
	va_list ap;
	char **av, **envp;

	va_start(ap, arg0);
	av = collect(arg0, &ap);
	if (av == NULL) {
		va_end(ap);
		errno = E2BIG;
		return -1;
	}
	envp = va_arg(ap, char **);
	va_end(ap);
	return execve(path, av, envp);
}

int execlp(const char *file, const char *arg0, ...)
{
	va_list ap;
	char **av;

	va_start(ap, arg0);
	av = collect(arg0, &ap);
	va_end(ap);
	if (av == NULL) {
		errno = E2BIG;
		return -1;
	}
	return execvp(file, av);
}

int fexecve(int fd, char *const argv[], char *const envp[])
{
	static fexecve_fn f;
	const char *m = mutant();
	int r;

	if (f == NULL)
		f = (fexecve_fn)dlsym(RTLD_NEXT, "fexecve");
	if (IS("fexecve_fails")) {
		errno = ENOEXEC;
		return -1;
	}
	if (m != NULL)
		before(m);
	r = f(fd, argv, envp);
	if (m != NULL)
		errno = map_errno(m, errno);
	return r;
}
