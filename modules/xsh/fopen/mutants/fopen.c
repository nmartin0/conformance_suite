/*
 * fopen.c - planted faults in fopen(), for mutation checks (CI only).
 *
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * A preload library; faults are named "<function>.<fault>" and apply only
 * while PC_PHASE is "test" (as set by pc.c in this process or a parent).
 */
#define _GNU_SOURCE 1
#include <dirent.h>
#include <dlfcn.h>
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

static char phase[16];

/* pc.c sets PC_PHASE with setenv(); remember it in this process. */
int setenv(const char *name, const char *value, int overwrite)
{
	static int (*real)(const char *, const char *, int);

	if (real == NULL)
		real = (int (*)(const char *, const char *, int))dlsym(RTLD_NEXT, "setenv");
	if (strcmp(name, "PC_PHASE") == 0 && strlen(value) < sizeof phase)
		strcpy(phase, value);
	return real(name, value, overwrite);
}

static const char *mutant_of(const char *prefix)
{
	const char *m = getenv("PC_MUTANT");
	size_t n = strlen(prefix);

	if (strcmp(phase, "test") != 0 || m == NULL || strncmp(m, prefix, n) != 0)
		return NULL;
	return m + n;
}

#define IS(name) (m != NULL && strcmp(m, name) == 0)
#define REAL(fn) static __typeof__(fn) *real_##fn; \
	if (real_##fn == NULL) real_##fn = (__typeof__(fn) *)dlsym(RTLD_NEXT, #fn)

#include <signal.h>
#include <stdio.h>
#include <sys/time.h>

/* fopen() rebuilt from open() and fdopen(), with one thing changed. */
static FILE *rebuilt(const char *path, const char *mode, int add, int drop, mode_t perm, int acc)
{
	REAL(fdopen);
	int plus = strchr(mode, '+') != NULL, fl, fd, end = 0;
	const char *base;
	FILE *f;

	switch (mode[0]) {
	case 'r': fl = plus ? O_RDWR : O_RDONLY; base = plus ? "r+" : "r"; break;
	case 'w': fl = (plus ? O_RDWR : O_WRONLY) | O_CREAT | O_TRUNC; base = plus ? "r+" : "w"; break;
	default: fl = (plus ? O_RDWR : O_WRONLY) | O_CREAT | O_APPEND; base = plus ? "r+" : "w"; break;
	}
	if (strchr(mode, 'e'))
		fl |= O_CLOEXEC;
	if (strchr(mode, 'x') && mode[0] != 'r')
		fl |= O_EXCL;
	if (acc != -1)
		fl = (fl & ~O_ACCMODE) | acc;
	if ((drop & O_APPEND) && (fl & O_APPEND))
		end = 1;
	fl = (fl | add) & ~drop;
	fd = open(path, fl, perm);
	if (fd == -1)
		return NULL;
	if (end)
		(void)lseek(fd, 0, SEEK_END);	/* once, at the start: later writes go wherever the stream is */
	f = real_fdopen(fd, base);
	if (f == NULL)
		(void)close(fd);
	return f;
}

static void parent_of(const char *path, char *dir, size_t n)
{
	const char *s = strrchr(path, '/');

	if (s == NULL || (size_t)(s - path) >= n)
		snprintf(dir, n, ".");
	else
		snprintf(dir, n, "%.*s", (int)(s - path), path);
}

FILE *fopen(const char *path, const char *mode)
{
	REAL(fopen);
	const char *m = mutant_of("fopen.");
	struct stat before, pst;
	char dir[4096];
	int existed, fd;
	FILE *f;

	if (m == NULL)
		return real_fopen(path, mode);
	existed = stat(path, &before) == 0;
	parent_of(path, dir, sizeof dir);
	if (stat(dir, &pst) != 0)
		memset(&pst, 0, sizeof pst);
	if (IS("r_truncates") && mode[0] == 'r')
		return rebuilt(path, mode, O_TRUNC, 0, 0666, O_RDWR);
	if (IS("r_at_end") && mode[0] == 'r' && (f = real_fopen(path, mode)) != NULL) {
		(void)fseek(f, 0, SEEK_END);
		return f;
	}
	if (IS("w_no_trunc") && mode[0] == 'w')
		return rebuilt(path, mode, 0, O_TRUNC, 0666, -1);
	if (IS("no_create") && mode[0] != 'r')
		return rebuilt(path, mode, 0, O_CREAT, 0666, -1);
	if (IS("a_no_append") && mode[0] == 'a')
		return rebuilt(path, mode, 0, O_APPEND, 0666, -1);
	if (IS("plus_ignored") && strchr(mode, '+'))
		return rebuilt(path, mode, 0, 0, 0666, mode[0] == 'r' ? O_RDONLY : O_WRONLY);
	if (IS("r_writable") && mode[0] == 'r' && !strchr(mode, '+'))
		return rebuilt(path, mode, 0, 0, 0666, O_RDWR);
	if (IS("adds_nonblock"))
		return rebuilt(path, mode, O_NONBLOCK, 0, 0666, -1);
	if (IS("adds_sync"))
		return rebuilt(path, mode, O_SYNC, 0, 0666, -1);
	if (IS("b_rejected") && strchr(mode, 'b')) {
		errno = EINVAL;
		return NULL;
	}
	if (IS("b_translates") && strchr(mode, 'b') && mode[0] == 'w' && (f = real_fopen(path, mode)) != NULL) {
		(void)fputc('\r', f);	/* a text-mode prefix that "b" should not have */
		return f;
	}
	if (IS("e_ignored") && strchr(mode, 'e'))
		return rebuilt(path, mode, 0, O_CLOEXEC, 0666, -1);
	if (IS("e_always"))
		return rebuilt(path, mode, O_CLOEXEC, 0, 0666, -1);
	if (IS("x_ignored") && strchr(mode, 'x'))
		return rebuilt(path, mode, 0, O_EXCL, 0666, -1);
	if (IS("x_rejected") && strchr(mode, 'x') && !existed) {
		errno = EINVAL;
		return NULL;
	}
	if (IS("x_private") && strchr(mode, 'x'))
		return rebuilt(path, mode, 0, 0, 0600, -1);
	if (IS("fixed_mode"))
		return rebuilt(path, mode, 0, 0, 0644, -1);
	if (IS("order_strict") && mode[1] != '\0' && mode[1] != '+' && strchr(mode, '+')) {
		errno = EINVAL;
		return NULL;
	}
	if (IS("slash_ignored") && path[0] != '\0' && path[strlen(path) - 1] == '/') {
		char p[4096];

		snprintf(p, sizeof p, "%s", path);
		while (strlen(p) > 1 && p[strlen(p) - 1] == '/')
			p[strlen(p) - 1] = '\0';
		return real_fopen(p, mode);
	}
	if (IS("r_creates") && mode[0] == 'r')
		return rebuilt(path, mode, O_CREAT, 0, 0666, -1);
	if (IS("dir_readonly") && existed && S_ISDIR(before.st_mode) && (mode[0] != 'r' || strchr(mode, '+'))) {
		REAL(fdopen);
		fd = open(path, O_RDONLY);
		return fd == -1 ? NULL : real_fdopen(fd, "r");
	}
	if (IS("restarts")) {
		sigset_t s, o;

		sigemptyset(&s);
		sigaddset(&s, SIGALRM);
		(void)sigprocmask(SIG_BLOCK, &s, &o);	/* the signal waits until the open is done */
		f = real_fopen(path, mode);
		(void)sigprocmask(SIG_SETMASK, &o, NULL);
		return f;
	}
	if (IS("name_einval")) {
		const unsigned char *c;

		for (c = (const unsigned char *)path; *c; c++)
			if (*c >= 0x80) {
				errno = EINVAL;
				return NULL;
			}
	}
	f = real_fopen(path, mode);
	if (f == NULL) {
		if (IS("wrong_errno"))
			errno = errno == EIO ? EINVAL : EIO;
		return NULL;
	}
	if (IS("error_set")) {
		if (mode[0] == 'r' && !strchr(mode, '+'))
			(void)fputc('x', f);	/* fails on a read-only stream: the error indicator is set */
		else if (!strchr(mode, '+'))
			(void)fgetc(f);
		else
			(void)fgetc(f), (void)fgetc(f);	/* at end of file: the end-of-file indicator is set */
	}
	if (IS("oriented"))
		(void)fwide(f, 1);
	if (IS("creat_old_times") && !existed && mode[0] != 'r') {
		struct timespec old[2] = { { 1000000000, 0 }, { 1000000000, 0 } };
		(void)futimens(fileno(f), old);
	}
	if (IS("restores_parent_times") && !existed && mode[0] != 'r' && pst.st_mtime != 0) {
		struct timespec old[2];
		old[0] = pst.st_atim;
		old[1] = pst.st_mtim;
		(void)utimensat(AT_FDCWD, dir, old, 0);
	}
	if (IS("trunc_old_times") && existed && mode[0] == 'w') {
		struct timespec old[2];
		old[0] = before.st_atim;
		old[1] = before.st_mtim;
		(void)futimens(fileno(f), old);
	}
	if (IS("high_fd") && (fd = fcntl(fileno(f), F_DUPFD, 64)) != -1) {
		REAL(fdopen);
		FILE *g = real_fdopen(fd, mode[0] == 'r' && !strchr(mode, '+') ? "r" : "r+");
		if (g != NULL) {
			(void)fclose(f);
			f = g;
		}
	}
	if (IS("full_tty") && isatty(fileno(f)))
		(void)setvbuf(f, NULL, _IOFBF, BUFSIZ);
	if (IS("unbuffered_file") && !isatty(fileno(f)))
		(void)setvbuf(f, NULL, _IONBF, 0);
	return f;
}
