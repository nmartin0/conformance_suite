/*
 * vfscanf.c - planted faults in vfscanf(), vscanf() and vsscanf(), for
 * mutation checks (CI only).
 *
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * A preload library; faults are named "vfscanf.<fault>" and apply only
 * while PC_PHASE is "test" (as set by pc.c in this process or a parent).
 * glibc compiles calls in strict modes to __isoc99_* names, so those are
 * interposed as well; the entry points take their symbol names from asm
 * labels because under _GNU_SOURCE glibc's <stdio.h> renames the plain
 * names itself.
 */
#define _GNU_SOURCE 1
#include <ctype.h>
#include <dlfcn.h>
#include <errno.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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

typedef int (*vs_fn)(const char *, const char *, va_list);
typedef int (*vf_fn)(FILE *, const char *, va_list);

static vs_fn real_vs(void)
{
	static vs_fn f;

	if (f == NULL)
		f = (vs_fn)dlsym(RTLD_NEXT, "__isoc99_vsscanf");
	if (f == NULL)
		f = (vs_fn)dlsym(RTLD_NEXT, "vsscanf");
	return f;
}

static vf_fn real_vf(void)
{
	static vf_fn f;

	if (f == NULL)
		f = (vf_fn)dlsym(RTLD_NEXT, "__isoc99_vfscanf");
	if (f == NULL)
		f = (vf_fn)dlsym(RTLD_NEXT, "vfscanf");
	return f;
}

/* The format with every "%n$" turned into a plain '%'. */
static const char *unnumbered(const char *fmt)
{
	static char out[512];
	size_t k = 0;
	const char *p;

	for (p = fmt; *p != '\0' && k < sizeof out - 1; p++) {
		out[k++] = *p;
		if (*p == '%' && isdigit((unsigned char)p[1])) {
			const char *q = p + 1;

			while (isdigit((unsigned char)*q))
				q++;
			if (*q == '$')
				p = q;
		}
	}
	out[k] = '\0';
	return out;
}

/* The format cut after its first conversion specification. */
static const char *first_only(const char *fmt)
{
	static char out[512];
	const char *p = fmt;
	size_t n;

	while (*p != '\0') {
		if (*p == '%' && p[1] == '%') {
			p += 2;
			continue;
		}
		if (*p == '%') {
			p++;
			while (*p != '\0' && strchr("diouxXaAeEfFgGsc[pnCS", *p) == NULL)
				p++;
			if (*p != '\0')
				p++;
			break;
		}
		p++;
	}
	n = (size_t)(p - fmt);
	if (n >= sizeof out)
		n = sizeof out - 1;
	memcpy(out, fmt, n);
	out[n] = '\0';
	return out;
}

static int scan(FILE *f, const char *s, const char *fmt, va_list ap)
{
	const char *m = mutant_of("vfscanf.");
	int r, e;

	if (m == NULL)
		return f != NULL ? real_vf()(f, fmt, ap) : real_vs()(s, fmt, ap);
	if (IS("numbered_ignored"))
		fmt = unnumbered(fmt);
	if (IS("stops_early"))
		fmt = first_only(fmt);
	if (IS("reads_stdin") && f != NULL && f != stdin)
		f = stdin;
	if (IS("own_buffer") && f == stdin) {
		/* reads descriptor 0 directly, behind the stream's back */
		static char buf[4096];
		ssize_t n = read(0, buf, sizeof buf - 1);

		buf[n > 0 ? n : 0] = '\0';
		f = NULL;
		s = buf;
	}
	r = f != NULL ? real_vf()(f, fmt, ap) : real_vs()(s, fmt, ap);
	e = errno;
	if (IS("eof_zero") && r == EOF)
		r = 0;
	if (IS("item_next_lost") && f != NULL && r > 0)
		(void)getc(f);
	if (f != NULL && ferror(f)) {
		if (IS("error_returns_zero") && r == EOF)
			r = 0;
		if (IS("error_clears_indicator"))
			clearerr(f);
	}
	errno = e;
	return r;
}

#define ENTRY_S(fn, sym) \
int fn(const char *restrict s, const char *restrict fmt, va_list ap) __asm__(sym); \
int fn(const char *restrict s, const char *restrict fmt, va_list ap) \
{ \
	return scan(NULL, s, fmt, ap); \
}
#define ENTRY_F(fn, sym) \
int fn(FILE *restrict f, const char *restrict fmt, va_list ap) __asm__(sym); \
int fn(FILE *restrict f, const char *restrict fmt, va_list ap) \
{ \
	return scan(f, NULL, fmt, ap); \
}
#define ENTRY_IN(fn, sym) \
int fn(const char *restrict fmt, va_list ap) __asm__(sym); \
int fn(const char *restrict fmt, va_list ap) \
{ \
	return scan(stdin, NULL, fmt, ap); \
}

ENTRY_S(pc_vsscanf, "vsscanf")
ENTRY_S(pc_vsscanf99, "__isoc99_vsscanf")
ENTRY_S(pc_vsscanf23, "__isoc23_vsscanf")
ENTRY_F(pc_vfscanf, "vfscanf")
ENTRY_F(pc_vfscanf99, "__isoc99_vfscanf")
ENTRY_F(pc_vfscanf23, "__isoc23_vfscanf")
ENTRY_IN(pc_vscanf, "vscanf")
ENTRY_IN(pc_vscanf99, "__isoc99_vscanf")
ENTRY_IN(pc_vscanf23, "__isoc23_vscanf")
