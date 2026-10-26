/*
 * clearerr.c - planted faults in clearerr(), for mutation checks (CI only).
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

void clearerr(FILE *f)
{
	REAL(clearerr);
	const char *m = mutant_of("clearerr.");

	if (m == NULL) {
		real_clearerr(f);
		return;
	}
	if (IS("eof_only")) {
		clear_bits(f, PC_EOF);	/* forgets the error indicator */
		return;
	}
	if (IS("error_only")) {
		clear_bits(f, PC_ERR);	/* forgets the end-of-file indicator */
		return;
	}
	if (IS("noop"))
		return;
	real_clearerr(f);
	if (IS("clears_errno"))
		errno = 0;
}
