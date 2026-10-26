/*
 * rewind.c - planted faults in rewind(), for mutation checks (CI only).
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

/* keeps_pushback: the last byte pushed back on a stream, in any phase. */
static FILE *pushed_f;
static int pushed_c;

int ungetc(int c, FILE *f)
{
	REAL(ungetc);
	int r = real_ungetc(c, f);

	if (r != EOF) {
		pushed_f = f;
		pushed_c = r;
	}
	return r;
}

void rewind(FILE *f)
{
	REAL(rewind);
	REAL(ungetc);
	const char *m = mutant_of("rewind.");
	unsigned bits;
	int e;

	if (m == NULL) {
		real_rewind(f);
		return;
	}
	if (IS("noop"))
		return;
	if (IS("clears_first")) {
		clearerr(f);	/* clears the indicators first, then seeks */
		(void)fseek(f, 0L, SEEK_SET);
		return;
	}
	if (IS("drops_pending"))
		__fpurge(f);	/* buffered output thrown away instead of written */
	bits = get_bits(f, PC_EOF | PC_ERR);
	e = errno;
	errno = 0;
	real_rewind(f);
	if (IS("keeps_eof") && (bits & PC_EOF))
		set_bits(f, PC_EOF);
	if (IS("keeps_error") && (bits & PC_ERR))
		set_bits(f, PC_ERR);
	if (IS("keeps_pushback") && pushed_f == f) {
		(void)real_ungetc(pushed_c, f);	/* the pushback survives */
		pushed_f = NULL;
	}
	if (errno != 0 && IS("wrong_errno"))
		errno = errno == EIO ? ENXIO : EIO;
	if (errno != 0 && IS("errno_hidden"))
		errno = 0;	/* the failure is not reported */
	if (errno == 0)
		errno = e;
}
