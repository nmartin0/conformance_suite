/*
 * exec.c - conforming variants of the exec family, for the variant checks
 * (CI only).
 *
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * A preload library; see core/pcvariant.h. Each variant says why the
 * standard allows it. glibc's execl(), execle(), execlp() and execv() reach
 * execve() internally, past any preload, so every member of the family is
 * wrapped; with no variant of ours active each wrapper calls the C
 * library's own function.
 */
#define _GNU_SOURCE 1
#include <errno.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include "pcvariant.h"

extern char **environ;

#define MAXARGS 4096

/*
 * "slow": every exec takes 2.5 s before the new image replaces the old
 * one. XSH exec states no bound on how long a successful exec takes (its
 * CPT text even allows for the time spent in the exec itself), so a busy
 * or slow system may take this long. Whatever happens meanwhile happens
 * to the old image: a timer that expires before exec deletes it, or a
 * signal, is the old image's.
 */
static void maybe_slow(const char *v)
{
	struct timespec ts = { 2, 500000000 };

	if (IS("slow"))
		while (nanosleep(&ts, &ts) == -1 && errno == EINTR)
			;
}

int execve(const char *path, char *const argv[], char *const envp[])
{
	const char *v = variant_of("exec.");
	REAL(execve);

	maybe_slow(v);
	return real_execve(path, argv, envp);
}

int execv(const char *path, char *const argv[])
{
	const char *v = variant_of("exec.");
	REAL(execv);

	if (v == NULL)
		return real_execv(path, argv);
	return execve(path, argv, environ);
}

int fexecve(int fd, char *const argv[], char *const envp[])
{
	const char *v = variant_of("exec.");
	REAL(fexecve);

	maybe_slow(v);
	return real_fexecve(fd, argv, envp);
}

/*
 * "sh_name": when execvp() or execlp() fall back to sh for a file in no
 * recognized format, the sh is found through confstr(_CS_PATH) (not
 * /bin/sh) and is given the caller's own arg0 as its argv[0]. XSH exec
 * leaves both the shell's pathname and that name unspecified, and its
 * RATIONALE names keeping the original arg0 as existing practice; sh then
 * sets $0 from the file operand, whatever argv[0] was.
 */
static const char *find_sh(void)
{
	static char found[4096];
	char cs[4096], *p, *c;
	size_t n = confstr(_CS_PATH, cs, sizeof cs);

	if (n == 0 || n > sizeof cs)
		return "/bin/sh";
	for (p = cs; p != NULL; p = c) {
		c = strchr(p, ':');
		if (c != NULL)
			*c++ = '\0';
		if (*p == '\0' || strlen(p) + 4 > sizeof found)
			continue;
		(void)snprintf(found, sizeof found, "%s/sh", p);
		if (access(found, X_OK) == 0)
			return found;
	}
	return "/bin/sh";
}

static int run_sh(const char *file, char *const argv[])
{
	static char *av[MAXARGS + 3];
	size_t n = 0, i;

	av[n++] = argv[0] != NULL ? argv[0] : "sh";
	av[n++] = (char *)file;
	for (i = 1; argv[0] != NULL && argv[i] != NULL; i++) {
		if (n == MAXARGS) {
			errno = E2BIG;
			return -1;
		}
		av[n++] = argv[i];
	}
	av[n] = NULL;
	return execve(find_sh(), av, environ);
}

static int try_one(const char *path, char *const argv[])
{
	int r = execve(path, argv, environ);

	if (r == -1 && errno == ENOEXEC)
		return run_sh(path, argv);
	return r;
}

/* execvp() as XSH exec and XBD 8.3 (PATH) describe it. */
static int own_execvp(const char *file, char *const argv[])
{
	char copy[8192], buf[8192], *p, *c;
	const char *path;
	int seen_eacces = 0;

	if (*file == '\0') {
		errno = ENOENT;
		return -1;
	}
	if (strchr(file, '/') != NULL)
		return try_one(file, argv);
	path = getenv("PATH");
	if (path == NULL) {
		/* implementation-defined: this system searches its _CS_PATH */
		size_t n = confstr(_CS_PATH, copy, sizeof copy);
		if (n == 0 || n > sizeof copy)
			(void)strcpy(copy, "/usr/bin:/bin");
	} else if (strlen(path) < sizeof copy) {
		(void)strcpy(copy, path);
	} else {
		errno = ENAMETOOLONG;
		return -1;
	}
	for (p = copy; p != NULL; p = c) {
		c = strchr(p, ':');
		if (c != NULL)
			*c++ = '\0';
		/* a zero-length prefix is the current directory */
		if (snprintf(buf, sizeof buf, "%s%s%s", *p ? p : "", *p ? "/" : "", file) >= (int)sizeof buf)
			continue;
		(void)try_one(buf, argv);
		if (errno == EACCES)
			seen_eacces = 1;
		else if (errno != ENOENT && errno != ENOTDIR && errno != ELOOP && errno != ENAMETOOLONG)
			return -1;
	}
	errno = seen_eacces ? EACCES : ENOENT;
	return -1;
}

int execvp(const char *file, char *const argv[])
{
	const char *v = variant_of("exec.");
	REAL(execvp);

	if (IS("sh_name"))
		return own_execvp(file, argv);
	if (v != NULL) {
		maybe_slow(v);
		return real_execvp(file, argv);
	}
	return real_execvp(file, argv);
}

/* Collects a variadic argument list ending in a null pointer. */
static char **collect(const char *arg0, va_list *ap)
{
	static char *av[MAXARGS + 1];
	size_t n = 0;
	char *a = (char *)arg0;

	while (a != NULL) {
		if (n == MAXARGS)
			return NULL;
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
