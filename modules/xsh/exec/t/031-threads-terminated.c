/*
 * exec from a thread ends the other threads without cleanup.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.exec.031
 * MUTANT    exec.cancels_threads
 */
#include "pc.h"
#include <dirent.h>
#include <errno.h>
#include <fcntl.h>
#include <limits.h>
#include <signal.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>
#include <pthread.h>

extern char **environ;

static char *self;		/* this program: the new process image */
static int rfd = -1;		/* in the image: where its report goes */
static int report[2];
static pid_t kid;
static char fdarg[16];
static char *iv[32];

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

/* Forks a child with a report pipe; returns 1 in the child. */
static int spawn_child(void)
{
	if (pipe(report) != 0)
		pc_unresolved("pipe failed");
	kid = fork();
	if (kid == -1)
		pc_unresolved("fork failed");
	if (kid == 0) {
		(void)close(report[0]);
		rfd = report[1];
		(void)snprintf(fdarg, sizeof fdarg, "%d", report[1]);
		return 1;
	}
	(void)close(report[1]);
	return 0;
}

/* In the child: arguments that run the image in mode m, then extra ones. */
static char **imgv(const char *m, ...)
{
	va_list ap;
	char *a;
	int n = 0;

	iv[n++] = self;
	iv[n++] = "img";
	iv[n++] = (char *)m;
	iv[n++] = fdarg;
	va_start(ap, m);
	while ((a = va_arg(ap, char *)) != NULL && n < 31)
		iv[n++] = a;
	va_end(ap);
	iv[n] = NULL;
	return iv;
}

/* In the child, when an exec function returned. */
_Noreturn static void returned(const char *fn)
{
	int e = errno;

	say("!%s returned -1 (%s)", fn, pc_errname(e));
	_exit(98);
}

/*
 * In the parent: reads the image's report to end of file and reaps the
 * child. A failed exec, or an image that did not end normally, is FAIL.
 */
static void collect(char *buf, size_t size)
{
	size_t n = 0;
	ssize_t r;
	int st;

	while (n < size - 1 && (r = read(report[0], buf + n, size - 1 - n)) > 0)
		n += (size_t)r;
	buf[n] = '\0';
	(void)close(report[0]);
	if (waitpid(kid, &st, 0) != kid)
		pc_unresolved("waitpid failed");
	if (WIFEXITED(st) && WEXITSTATUS(st) == 99)
		pc_unresolved("the child could not set up");
	if (buf[0] == '!')
		pc_fail("%s", buf + 1);
	pc_check(WIFEXITED(st) && WEXITSTATUS(st) == 0,
	    "the new image ended with status %#x (report \"%s\")", st, buf);
}

static void expect_ok(const char *buf)
{
	pc_check(strcmp(buf, "ok") == 0, "the new image reports: %s", buf);
}

/* In the child, after an exec that should fail returned r. */
_Noreturn static void failed(long r)
{
	int e = errno;

	if (r != -1)
		say("R:%ld", r);
	else
		say("E:%s", pc_errname(e));
	_exit(98);
}

/*
 * In the parent: the outcome of an exec that should fail in the child,
 * with errno err (or alt, when another condition also applies). The
 * image, if the exec wrongly succeeds, reports "ran".
 */
static void expect_fail(const char *what, int err, int alt)
{
	char buf[256];
	size_t n = 0;
	ssize_t r;
	int st;

	while (n < sizeof buf - 1 && (r = read(report[0], buf + n, sizeof buf - 1 - n)) > 0)
		n += (size_t)r;
	buf[n] = '\0';
	(void)close(report[0]);
	if (waitpid(kid, &st, 0) != kid)
		pc_unresolved("waitpid failed");
	if (WIFEXITED(st) && WEXITSTATUS(st) == 99)
		pc_unresolved("the child could not set up");
	if (strcmp(buf, "ran") == 0)
		pc_fail("%s succeeded: the new image ran", what);
	if (strncmp(buf, "R:", 2) == 0)
		pc_fail("%s returned %s, expected -1", what, buf + 2);
	pc_check(strncmp(buf, "E:", 2) == 0, "%s: no report from the child (status %#x)", what, st);
	if (strcmp(buf + 2, pc_errname(err)) == 0 || (alt != 0 && strcmp(buf + 2, pc_errname(alt)) == 0))
		return;
	if (alt != 0)
		pc_fail("%s failed with %s, expected %s or %s", what, buf + 2, pc_errname(err), pc_errname(alt));
	pc_fail("%s failed with %s, expected %s", what, buf + 2, pc_errname(err));
}

/* Copies this program to path with the given mode. */
static void copy_self(const char *path, mode_t mode)
{
	char b[8192];
	int in = open(self, O_RDONLY), out;
	ssize_t r;

	if (in == -1)
		pc_unresolved("cannot open %s", self);
	out = open(path, O_WRONLY | O_CREAT | O_TRUNC, 0700);
	if (out == -1)
		pc_unresolved("cannot create %s", path);
	while ((r = read(in, b, sizeof b)) > 0)
		if (write(out, b, (size_t)r) != r)
			pc_unresolved("short write to %s", path);
	(void)close(in);
	if (close(out) != 0 || chmod(path, mode) != 0)
		pc_unresolved("cannot finish %s", path);
}

static void image(const char *mode, int n, char **a)
{
	(void)mode;
	if (n < 1)
		say("missing argument");
	else if (atol(a[0]) != (long)getpid())
		say("process ID %ld in the new image, was %s", (long)getpid(), a[0]);
	else
		say("ok");
}

static pthread_key_t key;

static void cleanup(void *arg)
{
	(void)arg;
	say("cleanup handler ran; ");
}

static void destructor(void *arg)
{
	(void)arg;
	say("destructor ran; ");
}

static volatile sig_atomic_t ready, woken;

static void on_usr2(int sig)
{
	(void)sig;
}

/*
 * Sets its value and pushes its handler, says it is ready, then blocks in
 * pause(); each wakeup (a SIGUSR2 from main) is counted and it blocks again.
 */
static void *waiter(void *arg)
{
	(void)arg;
	(void)pthread_setspecific(key, &key);
	pthread_cleanup_push(cleanup, NULL);
	ready = 1;
	for (;;) {
		(void)pause();
		woken++;
	}
	pthread_cleanup_pop(0);
	return NULL;
}

/* Waits up to 10 s for *flag; returns whether it was set. */
static int wait_flag(volatile sig_atomic_t *flag)
{
	int i;

	for (i = 0; i < 1000 && !*flag; i++) {
		struct timespec ts = { 0, 10000000 };
		(void)nanosleep(&ts, NULL);
	}
	return *flag != 0;
}

static char pidarg[24];

static void *execer(void *arg)
{
	(void)arg;
	execv(self, imgv("thr", pidarg, (char *)NULL));
	returned("execv");
}

int main(int argc, char **argv)
{
	if (argc >= 4 && strcmp(argv[1], "img") == 0) {
		rfd = atoi(argv[3]);
		self = argv[0];
		image(argv[2], argc - 4, argv + 4);
		return 0;
	}
	pc_begin("XSH.exec.031");
	pc_check(argc >= 1 && argv[0][0] == '/', "argv[0] is not an absolute path");
	self = argv[0];
	pc_test();
	if (spawn_child()) {
		pthread_t w, e;
		struct sigaction sa;
		sigset_t u2;
		(void)snprintf(pidarg, sizeof pidarg, "%ld", (long)getpid());
		memset(&sa, 0, sizeof sa);
		sigemptyset(&sa.sa_mask);
		sa.sa_handler = on_usr2;
		/* only the waiter takes SIGUSR2: the others block it */
		sigemptyset(&u2);
		sigaddset(&u2, SIGUSR2);
		if (sigaction(SIGUSR2, &sa, NULL) != 0 || pthread_key_create(&key, destructor) != 0 ||
		    pthread_create(&w, NULL, waiter, NULL) != 0 ||
		    pthread_sigmask(SIG_BLOCK, &u2, NULL) != 0)
			_exit(99);
		/*
		 * The waiter is in place (value set, handler pushed) and has
		 * blocked in pause(): a SIGUSR2 sent to it woke it there. If
		 * that cannot be seen within 10 s, the child cannot set up.
		 */
		if (!wait_flag(&ready))
			_exit(99);
		{
			/* resent: one that comes before pause() wakes nothing */
			int i;
			for (i = 0; i < 1000 && !woken; i++) {
				struct timespec ts = { 0, 10000000 };
				if (pthread_kill(w, SIGUSR2) != 0)
					_exit(99);
				(void)nanosleep(&ts, NULL);
			}
			if (!woken)
				_exit(99);
		}
		if (pthread_create(&e, NULL, execer, NULL) != 0)
			_exit(99);
		for (;;)
			(void)pause();
	}
	{
		char buf[256];
		collect(buf, sizeof buf);
		expect_ok(buf);
	}
	pc_pass();
}
