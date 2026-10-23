/*
 * Per-process timers are deleted by exec.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.exec.029
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
	struct timespec ts = { 3, 0 };

	(void)mode;
	(void)n;
	(void)a;
	while (nanosleep(&ts, &ts) == -1 && errno == EINTR)
		;
	say("ok");
}

int main(int argc, char **argv)
{
	if (argc >= 4 && strcmp(argv[1], "img") == 0) {
		rfd = atoi(argv[3]);
		self = argv[0];
		image(argv[2], argc - 4, argv + 4);
		return 0;
	}
	pc_begin("XSH.exec.029");
	pc_check(argc >= 1 && argv[0][0] == '/', "argv[0] is not an absolute path");
	self = argv[0];
	pc_test();
	if (spawn_child()) {
		timer_t tid;
		struct sigevent ev;
		struct itimerspec it;
		memset(&ev, 0, sizeof ev);
		ev.sigev_notify = SIGEV_SIGNAL;
		ev.sigev_signo = SIGUSR1;
		memset(&it, 0, sizeof it);
		it.it_value.tv_sec = 2;	/* long enough for a slow exec */
		if (timer_create(CLOCK_REALTIME, &ev, &tid) != 0 || timer_settime(tid, 0, &it, NULL) != 0)
			_exit(99);
		execv(self, imgv("timer", (char *)NULL));
		returned("execv");
	}
	{
		char buf[256];
		size_t n = 0;
		ssize_t r;
		int st;
		while (n < sizeof buf - 1 && (r = read(report[0], buf + n, sizeof buf - 1 - n)) > 0)
			n += (size_t)r;
		buf[n] = '\0';
		if (waitpid(kid, &st, 0) != kid)
			pc_unresolved("waitpid failed");
		if (WIFEXITED(st) && WEXITSTATUS(st) == 99)
			pc_unresolved("the child could not set up the timer");
		if (buf[0] == '!')
			pc_fail("%s", buf + 1);
		pc_check(!(WIFSIGNALED(st) && WTERMSIG(st) == SIGUSR1), "the old image's timer fired in the new image");
		pc_check(WIFEXITED(st) && WEXITSTATUS(st) == 0, "the new image ended with status %#x", st);
		expect_ok(buf);
	}
	pc_pass();
}
