/*
 * vfprintf.c - planted faults in the va_list forms of the fprintf() family,
 * for mutation checks (CI only).
 *
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * A preload library; faults are named "vfprintf.<fault>" and apply only
 * while PC_PHASE is "test" (as set by pc.c in this process or a parent).
 */
#define _GNU_SOURCE 1
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
#define REAL(fn) static __typeof__(fn) *real_##fn; \
	if (real_##fn == NULL) real_##fn = (__typeof__(fn) *)dlsym(RTLD_NEXT, #fn)

/*
 * "skips_arg": the list is advanced by one int before the format uses it,
 * as an implementation that fetched an argument too many would.
 */
#define LIST(m, cp, ap) do { \
	va_copy(cp, ap); \
	if (IS("skips_arg")) \
		(void)va_arg(cp, int); \
} while (0)

/*
 * "pos_ignored": the "n$" and "*m$" positions are dropped, so that numbered
 * conversions take the arguments in order, as an implementation without
 * them would.
 */
static const char *unnumbered(const char *fmt, char *buf, size_t size)
{
	const char *p = fmt;
	char *q = buf;
	int in = 0;

	while (*p != '\0' && q < buf + size - 1) {
		if (*p == '%') {
			in = !in || p[-1] != '%';
			*q++ = *p++;
			continue;
		}
		if (in && *p >= '1' && *p <= '9') {
			const char *d = p;

			while (*d >= '0' && *d <= '9')
				d++;
			if (*d == '$') {
				p = d + 1;
				continue;
			}
		}
		if (in && strchr("diouxXfFeEgGaAcspnCS", *p) != NULL)
			in = 0;
		*q++ = *p++;
	}
	*q = '\0';
	return buf;
}

int vsnprintf(char *restrict s, size_t n, const char *restrict fmt, va_list ap)
{
	REAL(vsnprintf);
	const char *m = mutant_of("vfprintf.");
	va_list cp;
	int r, e = errno;

	if (m == NULL)
		return real_vsnprintf(s, n, fmt, ap);
	LIST(m, cp, ap);
	if (IS("pos_ignored")) {
		static char nb[512];

		fmt = unnumbered(fmt, nb, sizeof nb);
	}
	if (IS("vsn_ignores_n") && n > 0) {
		/* writes the whole result whatever n says */
		r = real_vsnprintf(NULL, 0, fmt, cp);
		va_end(cp);
		va_copy(cp, ap);
		if (r >= 0)
			r = real_vsnprintf(s, (size_t)r + 1, fmt, cp);
	} else {
		r = real_vsnprintf(s, n, fmt, cp);
	}
	va_end(cp);
	if (IS("vsn_returns_written") && r >= 0 && n > 0 && (size_t)r >= n)
		r = (int)n - 1;
	if (IS("overflow_no_errno") && r < 0 && errno == EOVERFLOW)
		errno = e;
	return r;
}

int vsprintf(char *restrict s, const char *restrict fmt, va_list ap)
{
	REAL(vsprintf);
	const char *m = mutant_of("vfprintf.");
	va_list cp;
	int r;

	if (m == NULL)
		return real_vsprintf(s, fmt, ap);
	LIST(m, cp, ap);
	if (IS("vs_no_nul")) {
		char keep[1], *b;
		REAL(vsnprintf);

		r = real_vsnprintf(keep, sizeof keep, fmt, cp);
		va_end(cp);
		va_copy(cp, ap);
		b = r >= 0 ? malloc((size_t)r + 1) : NULL;
		if (b != NULL) {
			(void)real_vsnprintf(b, (size_t)r + 1, fmt, cp);
			memcpy(s, b, (size_t)r);	/* the null byte is not stored */
			free(b);
		}
	} else {
		r = real_vsprintf(s, fmt, cp);
	}
	va_end(cp);
	return r;
}

int vfprintf(FILE *restrict f, const char *restrict fmt, va_list ap)
{
	REAL(vfprintf);
	const char *m = mutant_of("vfprintf.");
	va_list cp;
	int r;

	if (m == NULL)
		return real_vfprintf(f, fmt, ap);
	LIST(m, cp, ap);
	r = real_vfprintf(IS("vf_to_stdout") ? stdout : f, fmt, cp);
	va_end(cp);
	return IS("vf_returns_zero") && r > 0 ? 0 : r;
}

int vprintf(const char *restrict fmt, va_list ap)
{
	REAL(vprintf);
	REAL(vfprintf);
	const char *m = mutant_of("vfprintf.");
	va_list cp;
	int r;

	if (m == NULL)
		return real_vprintf(fmt, ap);
	LIST(m, cp, ap);
	r = real_vfprintf(IS("vp_to_stderr") ? stderr : stdout, fmt, cp);
	va_end(cp);
	return r;
}

int vdprintf(int fd, const char *restrict fmt, va_list ap)
{
	REAL(vdprintf);
	const char *m = mutant_of("vfprintf.");
	va_list cp;
	int r;

	if (m == NULL)
		return real_vdprintf(fd, fmt, ap);
	LIST(m, cp, ap);
	if (IS("vd_lost")) {
		REAL(vsnprintf);

		r = real_vsnprintf(NULL, 0, fmt, cp);	/* formatted, never written */
	} else {
		r = real_vdprintf(fd, fmt, cp);
	}
	va_end(cp);
	return IS("vd_returns_zero") && r > 0 ? 0 : r;
}

int vasprintf(char **restrict ptr, const char *restrict fmt, va_list ap)
{
	REAL(vasprintf);
	const char *m = mutant_of("vfprintf.");
	va_list cp;
	int r;

	if (m == NULL)
		return real_vasprintf(ptr, fmt, ap);
	LIST(m, cp, ap);
	r = real_vasprintf(ptr, fmt, cp);
	va_end(cp);
	return IS("vas_returns_size") && r >= 0 ? r + 1 : r;
}
