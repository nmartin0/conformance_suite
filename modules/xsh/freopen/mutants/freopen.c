/*
 * freopen.c - planted faults in freopen(), for mutation checks (CI only).
 *
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * A preload library; faults are named "<function>.<fault>" and apply only
 * while PC_PHASE is "test" (as set by pc.c in this process or a parent).
 */
#define _GNU_SOURCE 1
#include <dlfcn.h>
#include <errno.h>
#include <fcntl.h>
#include <signal.h>
#include <stdio.h>
#include <stdio_ext.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <wchar.h>

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

/*
 * A stream's indicators as the program sees them. The library's own are
 * left alone; a fault is planted by keeping an indicator the library does
 * not report, or by hiding one it does, in a small table that feof(),
 * ferror() and clearerr() consult. clearerr() forgets a stream's entry.
 */
#define PC_EOF 1u
#define PC_ERR 2u

static struct { FILE *f; unsigned kept, hidden; } ovl[16];

static int ovl_find(FILE *f, int make)
{
	int i;

	for (i = 0; i < 16; i++)
		if (ovl[i].f == f)
			return i;
	if (make)
		for (i = 0; i < 16; i++)
			if (ovl[i].f == NULL) {
				ovl[i].f = f;
				return i;
			}
	return -1;
}

static unsigned ovl_apply(FILE *f, unsigned seen)
{
	int i = ovl_find(f, 0);

	return i < 0 ? seen : (seen | ovl[i].kept) & ~ovl[i].hidden;
}

static void ovl_forget(FILE *f)
{
	int i = ovl_find(f, 0);

	if (i >= 0)
		ovl[i].f = NULL, ovl[i].kept = ovl[i].hidden = 0;
}

int feof(FILE *f)
{
	REAL(feof);

	return (ovl_apply(f, real_feof(f) ? PC_EOF : 0) & PC_EOF) != 0;
}

int ferror(FILE *f)
{
	REAL(ferror);

	return (ovl_apply(f, real_ferror(f) ? PC_ERR : 0) & PC_ERR) != 0;
}

/* The bits of b that f reports. */
static unsigned get_bits(FILE *f, unsigned b)
{
	return ((feof(f) ? PC_EOF : 0) | (ferror(f) ? PC_ERR : 0)) & b;
}

/* f reports the indicators in b, whatever the library says. */
static void set_bits(FILE *f, unsigned b)
{
	int i = ovl_find(f, 1);

	if (i >= 0)
		ovl[i].kept |= b, ovl[i].hidden &= ~b;
}

/* f does not report the indicators in b, whatever the library says. */
static void clear_bits(FILE *f, unsigned b)
{
	int i = ovl_find(f, 1);

	if (i >= 0)
		ovl[i].hidden |= b, ovl[i].kept &= ~b;
}

void clearerr(FILE *f)
{
	REAL(clearerr);

	ovl_forget(f);
	real_clearerr(f);
}

/* mode with the characters in drop left out, or with c put after the first. */
static const char *edit(const char *mode, const char *drop, char add, char *buf, size_t size)
{
	size_t i, n = 0;

	for (i = 0; mode[i] != '\0' && n + 2 < size; i++) {
		if (strchr(drop, mode[i]) != NULL)
			continue;
		buf[n++] = mode[i];
		if (i == 0 && add != '\0' && strchr(mode, add) == NULL)
			buf[n++] = add;
	}
	buf[n] = '\0';
	return buf;
}

FILE *freopen(const char *restrict path, const char *restrict mode, FILE *restrict f)
{
	REAL(freopen);
	const char *m = mutant_of("freopen.");
	char mb[16];
	FILE *r;
	int e, fd, t;
	unsigned bits;

	if (m == NULL)
		return real_freopen(path, mode, f);
	if (IS("new_stream") && path != NULL)
		return fopen(path, mode);	/* a new stream; the old one is left alone */
	if (IS("keeps_position")) {
		off_t at;

		e = errno;
		at = ftello(f);
		errno = e;
		r = real_freopen(path, mode, f);
		if (r != NULL && at > 0)
			(void)fseeko(r, at, SEEK_SET);	/* the old position carried over */
		return r;
	}
	if (IS("drops_pending"))
		__fpurge(f);	/* buffered output thrown away instead of flushed */
	if (IS("keeps_flags") || (IS("null_keeps_flags") && path == NULL)) {
		bits = get_bits(f, PC_EOF | PC_ERR);
		r = real_freopen(path, mode, f);
		if (r != NULL)
			set_bits(r, bits);	/* the indicators survive the reopen */
		return r;
	}
	if (IS("close_fails") && path != NULL) {
		e = errno;
		fd = fileno(f);
		if (fd >= 0 && fcntl(fd, F_GETFD) == -1) {
			errno = EBADF;	/* the failed close is reported */
			return NULL;
		}
		errno = e;
	}
	if (IS("opens_first") && path != NULL) {
		int probe = open(path, mode[0] == 'r' && strchr(mode, '+') == NULL ? O_RDONLY : O_RDWR);

		if (probe == -1 && errno == EMFILE)
			return NULL;	/* the new file opened before the old descriptor is freed */
		if (probe != -1)
			(void)close(probe);
	}
	if (IS("flush_fails") && fflush(f) == EOF)
		return NULL;	/* the failed flush is reported */
	if ((IS("keeps_old") || IS("returns_stream")) && path != NULL) {
		FILE *probe = fopen(path, mode);

		if (probe == NULL)
			return IS("keeps_old") ? NULL : f;	/* fails before the original is touched */
		(void)fclose(probe);
	}
	if (IS("rdwr_always"))
		mode = edit(mode, "", '+', mb, sizeof mb);	/* every mode opened for update */
	if (IS("e_ignored"))
		mode = edit(mode, "e", '\0', mb, sizeof mb);
	if (IS("x_ignored"))
		mode = edit(mode, "x", '\0', mb, sizeof mb);
	if (IS("no_truncate") && mode[0] == 'w') {
		mode = edit(mode, "", '\0', mb, sizeof mb);
		mb[0] = 'a';	/* "w" opened without O_TRUNC */
	}
	if (IS("null_no_reopen") && path == NULL) {
		(void)fflush(f);
		clearerr(f);
		return f;	/* the mode is "changed" without opening the file again */
	}
	if (IS("mode_0600")) {
		mode_t old = umask(077);

		r = real_freopen(path, mode, f);
		(void)umask(old);
		return r;
	}
	if (IS("leaks_old")) {
		fd = fileno(f);
		t = fd >= 0 ? dup(fd) : -1;
		r = real_freopen(path, mode, f);
		if (t >= 0) {
			if (fcntl(fd, F_GETFD) == -1)
				(void)dup2(t, fd);	/* the old descriptor is never closed */
			(void)close(t);
		}
		return r;
	}
	if (IS("keeps_orientation")) {
		int o = fwide(f, 0);

		r = real_freopen(path, mode, f);
		if (r != NULL && o != 0)
			(void)fwide(r, o);
		return r;
	}
	r = real_freopen(path, mode, f);
	if (r != NULL && IS("cloexec_always"))
		(void)fcntl(fileno(r), F_SETFD, FD_CLOEXEC);
	if (r == NULL && IS("wrong_errno"))
		errno = errno == EIO ? ENXIO : EIO;
	if (r == NULL && IS("returns_stream"))
		return f;
	return r;
}
