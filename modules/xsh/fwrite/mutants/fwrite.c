/*
 * fwrite.c - planted faults in fwrite(), for mutation checks (CI only).
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
 * A stream's indicators, set or cleared behind the C library's back to
 * plant a fault. Both reference C libraries keep them in the first word of
 * the FILE object, with the same bits: glibc's _flags (_IO_EOF_SEEN 0x10,
 * _IO_ERR_SEEN 0x20) and musl's flags (F_EOF 16, F_ERR 32).
 */
#define PC_EOF 0x10u
#define PC_ERR 0x20u

static void set_bits(FILE *f, unsigned b)
{
	*(unsigned *)(void *)f |= b;
}

static void clear_bits(FILE *f, unsigned b)
{
	*(unsigned *)(void *)f &= ~b;
}

/* The bits of b that are set on f. */
static unsigned get_bits(FILE *f, unsigned b)
{
	return *(unsigned *)(void *)f & b;
}

/* old_times: the descriptor whose times the next flush puts back. */
static int stale_fd = -1;
static struct timespec stale[2];

size_t fwrite(const void *restrict ptr, size_t size, size_t n, FILE *restrict f)
{
	REAL(fwrite);
	const char *m = mutant_of("fwrite.");
	const unsigned char *b = ptr;
	size_t r, i, k;

	if (m == NULL)
		return real_fwrite(ptr, size, n, f);
	if (size == 0 || n == 0) {
		if (IS("zero_writes")) {
			(void)real_fwrite(ptr, 1, 1, f);	/* an empty request still writes one byte */
			return 0;
		}
		if (IS("zero_clears")) {
			clearerr(f);	/* resets the stream's state on entry */
			return 0;
		}
	}
	if (IS("stops_at_nul")) {
		const unsigned char *z = memchr(b, 0, size * n);

		if (z != NULL)
			(void)real_fwrite(b, 1, (size_t)(z - b), f);	/* written as a string */
		else
			(void)real_fwrite(b, size, n, f);
		return n;
	}
	if (IS("swapped") && size > 1) {
		unsigned char e[64];

		if (size > sizeof e)
			return real_fwrite(ptr, size, n, f);
		for (k = 0; k < n; k++) {
			for (i = 0; i < size; i++)
				e[i] = b[k * size + size - 1 - i];
			if (real_fwrite(e, size, 1, f) != 1)
				return k;
		}
		return n;
	}
	if (IS("appends"))
		(void)fseek(f, 0, SEEK_END);	/* writes go to the end, as if opened for append */
	if (IS("capped") && size > 0 && size * n > 4096)
		return real_fwrite(ptr, size, 4096 / size, f);	/* one buffer's worth per call */
	if (IS("old_times") && fileno(f) >= 0) {
		struct stat st;

		if (fstat(fileno(f), &st) == 0) {
			stale_fd = fileno(f);
			stale[0] = st.st_atim;
			stale[1] = st.st_mtim;
		}
	}
	r = real_fwrite(ptr, size, n, f);
	if (IS("bytes_count"))
		return r * size;
	if (IS("no_error_flag") && r < n)
		clear_bits(f, PC_ERR);
	if (IS("wrong_errno") && r < n && ferror(f))
		errno = errno == EIO ? ENXIO : EIO;
	if (IS("error_full_count") && r < n && ferror(f))
		return n;	/* reports success for data it failed to write */
	return r;
}

/* old_times: the data reaches the file without the times being marked. */
int fflush(FILE *f)
{
	REAL(fflush);
	const char *m = mutant_of("fwrite.");
	int r = real_fflush(f);

	if (IS("old_times") && f != NULL && stale_fd >= 0 && fileno(f) == stale_fd)
		(void)futimens(stale_fd, stale);
	return r;
}
