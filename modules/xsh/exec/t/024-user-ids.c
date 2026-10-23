/*
 * Real and effective IDs and supplementary groups are kept across exec.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.exec.024
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
	(void)n;
	(void)a;
	gid_t g[NGROUPS_MAX + 1];
	int ng = getgroups(NGROUPS_MAX + 1, g), i;
	long sum = 0;
	for (i = 0; i < ng; i++)
		sum += (long)g[i];
	if (n < 6)
		say("missing arguments");
	else if (atol(a[0]) != (long)getuid() || atol(a[1]) != (long)geteuid())
		say("user IDs %ld/%ld, were %s/%s", (long)getuid(), (long)geteuid(), a[0], a[1]);
	else if (atol(a[2]) != (long)getgid() || atol(a[3]) != (long)getegid())
		say("group IDs %ld/%ld, were %s/%s", (long)getgid(), (long)getegid(), a[2], a[3]);
	else if (ng != atoi(a[4]) || sum != atol(a[5]))
		say("supplementary groups changed (%d groups, were %s)", ng, a[4]);
	else
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
	pc_begin("XSH.exec.024");
	pc_check(argc >= 1 && argv[0][0] == '/', "argv[0] is not an absolute path");
	self = argv[0];
	pc_test();
	if (spawn_child()) {
		static char ids[4][24], grp[2][24];
		gid_t g[NGROUPS_MAX + 1];
		int ng = getgroups(NGROUPS_MAX + 1, g), i;
		long sum = 0;
		if (ng < 0)
			_exit(99);
		for (i = 0; i < ng; i++)
			sum += (long)g[i];
		(void)snprintf(ids[0], sizeof ids[0], "%ld", (long)getuid());
		(void)snprintf(ids[1], sizeof ids[1], "%ld", (long)geteuid());
		(void)snprintf(ids[2], sizeof ids[2], "%ld", (long)getgid());
		(void)snprintf(ids[3], sizeof ids[3], "%ld", (long)getegid());
		(void)snprintf(grp[0], sizeof grp[0], "%d", ng);
		(void)snprintf(grp[1], sizeof grp[1], "%ld", sum);
		execv(self, imgv("attr", ids[0], ids[1], ids[2], ids[3], grp[0], grp[1], (char *)NULL));
		returned("execv");
	}
	{
		char buf[512];
		collect(buf, sizeof buf);
		expect_ok(buf);
	}
	pc_pass();
}
