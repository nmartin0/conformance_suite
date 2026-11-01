/*
 * fclose.c - planted faults in fclose(), for mutation checks (CI only).
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
#include <time.h>

/*
 * Buffered data, found by comparing the stream's position with the
 * descriptor's offset: output not yet written leaves the descriptor behind
 * the stream, input read ahead leaves it in front.
 */
static long long ahead(FILE *f)
{
	REAL(ftello);
	int e = errno, fd = fileno(f);
	off_t s = real_ftello(f), d = fd >= 0 ? lseek(fd, 0, SEEK_CUR) : -1;

	errno = e;
	return s == -1 || d == -1 ? 0 : (long long)(d - s);
}

/*
 * Output the stream has buffered but not yet written, found by comparing
 * the stream's position with the descriptor's offset. A fault that
 * "drops" it lets the real call write it and then puts the file back as
 * it was: the bytes it overwrote restored, any growth cut off.
 */
struct undo {
	int fd;
	off_t at, size;
	char *saved;
	size_t n;
};

static void undo_begin(FILE *f, struct undo *u)
{
	REAL(ftello);
	struct stat st;
	int e = errno, fd = fileno(f);
	off_t s = real_ftello(f), d = fd >= 0 ? lseek(fd, 0, SEEK_CUR) : -1;

	u->fd = -1;
	u->saved = NULL;
	u->n = 0;
	if (s != -1 && d != -1 && s > d && fstat(fd, &st) == 0 && S_ISREG(st.st_mode)) {
		u->at = d;
		u->size = st.st_size;
		if (d < st.st_size) {
			u->n = (size_t)(st.st_size - d < s - d ? st.st_size - d : s - d);
			u->saved = malloc(u->n);
			if (u->saved == NULL || pread(fd, u->saved, u->n, d) != (ssize_t)u->n) {
				free(u->saved);
				u->saved = NULL;
				u->n = 0;
			}
		}
		u->fd = dup(fd);
	}
	errno = e;
}

static void undo_end(struct undo *u)
{
	int e = errno;

	if (u->fd >= 0) {
		if (u->saved != NULL)
			(void)pwrite(u->fd, u->saved, u->n, u->at);
		(void)ftruncate(u->fd, u->size);
		(void)close(u->fd);
	}
	free(u->saved);
	errno = e;
}

int fclose(FILE *f)
{
	REAL(fclose);
	const char *m = mutant_of("fclose.");
	struct stat st;
	struct undo u;
	int r, e, fd = fileno(f), keep = -1, fl;

	if (m == NULL)
		return real_fclose(f);
	fl = fcntl(fd, F_GETFL);
	if (IS("stale_offset") && ahead(f) < 0)
		(void)lseek(fd, 0, SEEK_SET);	/* writes where the stream started, as if no other handle had moved */
	if (IS("no_input_sync") && fl != -1 && ((fl & O_ACCMODE) == O_RDONLY || ahead(f) > 0)) {
		off_t at = lseek(fd, 0, SEEK_CUR);
		int d = dup(fd);
		r = real_fclose(f);
		if (d >= 0) {
			(void)lseek(d, at, SEEK_SET);	/* the offset stays where the buffer left it */
			(void)close(d);
		}
		return r;
	}
	if (IS("mtime_kept") && fstat(fd, &st) == 0) {
		struct timespec v[2];
		int d = dup(fd);
		r = real_fclose(f);
		v[0] = st.st_atim;
		v[1] = st.st_mtim;
		if (d >= 0) {
			(void)futimens(d, v);
			(void)close(d);
		}
		return r;
	}
	if (IS("leaks_fd") || IS("leaks_on_error"))
		keep = dup(fd);
	u.fd = -1;
	u.saved = NULL;
	if (IS("no_flush"))
		undo_begin(f, &u);	/* buffered output is dropped */
	r = real_fclose(f);
	undo_end(&u);
	e = errno;
	if (keep >= 0) {
		if (IS("leaks_fd") || r == EOF)
			(void)dup2(keep, fd);	/* the descriptor stays open */
		(void)close(keep);
	}
	if (r == 0 && IS("returns_nonzero"))
		return 1;
	if (r == EOF && IS("errno_lost"))
		e = 0;
	if (r == EOF && IS("success_on_error"))
		return 0;
	errno = e;
	return r;
}
