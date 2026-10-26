/*
 * fread.c - planted faults in fread(), for mutation checks (CI only).
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

static int is_fifo(FILE *f)
{
	struct stat st;
	int e = errno, fd = fileno(f), r;

	r = fd >= 0 && fstat(fd, &st) == 0 && S_ISFIFO(st.st_mode);
	errno = e;
	return r;
}

size_t fread(void *restrict ptr, size_t size, size_t n, FILE *restrict f)
{
	REAL(fread);
	const char *m = mutant_of("fread.");
	unsigned char *b = ptr;
	struct stat st0;
	off_t before = -1, after;
	size_t r, i, k;
	int have = 0;

	if (m == NULL)
		return real_fread(ptr, size, n, f);
	if (size == 0 || n == 0) {
		if (IS("zero_reads")) {
			(void)real_fread(ptr, 1, 1, f);	/* treats an empty request as a request for one byte */
			return 0;
		}
		if (IS("zero_clears")) {
			clearerr(f);	/* resets the stream's state on entry, before looking at the sizes */
			return 0;
		}
	}
	if (IS("one_read") && size > 0 && is_fifo(f)) {
		/* one read() of the whole request, taking whatever the pipe has */
		ssize_t got = read(fileno(f), ptr, size * n);

		if (got < 0) {
			set_bits(f, PC_ERR);
			return 0;
		}
		if (got == 0)
			set_bits(f, PC_EOF);
		return (size_t)got / size;
	}
	if (IS("partial_counted")) {
		int e = errno;

		before = ftello(f);
		errno = e;
	}
	if (IS("no_atime") && fileno(f) >= 0)
		have = fstat(fileno(f), &st0) == 0;
	r = real_fread(ptr, size, n, f);
	if (IS("short_one") && r > 1)
		return r - 1;
	if (IS("bytes_count"))
		return r * size;
	if (IS("skips") && r > 0)
		(void)fgetc(f);	/* an off-by-one in the buffer pointer: one byte too many is consumed */
	if (IS("stops_at_nul") && r > 0) {
		unsigned char *z = memchr(b, 0, r * size);

		if (z != NULL)
			memset(z, 0, (size_t)(b + r * size - z));	/* copied as a string */
	}
	if (IS("high_bit_lost"))
		for (i = 0; i < r * size; i++)
			b[i] &= 0x7f;
	if (IS("swapped") && size > 1)
		for (k = 0; k < r; k++)
			for (i = 0; i < size / 2; i++) {
				unsigned char t = b[k * size + i];

				b[k * size + i] = b[k * size + size - 1 - i];
				b[k * size + size - 1 - i] = t;
			}
	if (IS("partial_counted") && r < n && size > 1 && before != -1) {
		int e = errno;

		after = ftello(f);
		errno = e;
		if (after != -1 && (size_t)(after - before) % size != 0)
			return r + 1;	/* a partly read element is counted */
	}
	if (IS("eof_not_set") && r < n)
		clear_bits(f, PC_EOF);
	if (IS("no_error_flag") && r < n)
		clear_bits(f, PC_ERR);
	if (IS("wrong_errno") && r < n && ferror(f))
		errno = errno == EIO ? ENXIO : EIO;
	if (IS("no_atime") && have && r > 0) {
		struct timespec ts[2];

		ts[0] = st0.st_atim;
		ts[1].tv_sec = 0;
		ts[1].tv_nsec = UTIME_OMIT;
		(void)futimens(fileno(f), ts);	/* the read leaves the access time alone */
	}
	return r;
}
