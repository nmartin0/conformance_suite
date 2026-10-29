/*
 * mbrtowc.c - planted faults in mbrtowc(), for the UTF-8 checks (CI only).
 *
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * A preload library; faults are named "mbrtowc.<fault>" and apply only while
 * PC_PHASE is "test". Each makes a decoder accept a sequence that RFC 3629
 * forbids.
 */
#define _GNU_SOURCE 1
#include <dlfcn.h>
#include <stdlib.h>
#include <string.h>
#include <wchar.h>

typedef size_t (*fn_t)(wchar_t *, const char *, size_t, mbstate_t *);

static const char *mutant(void)
{
	const char *ph = getenv("PC_PHASE");
	const char *m = getenv("PC_MUTANT");

	if (ph == NULL || strcmp(ph, "test") != 0 || m == NULL || strncmp(m, "mbrtowc.", 8) != 0)
		return NULL;
	return m + 8;
}

#define IS(name) (m != NULL && strcmp(m, name) == 0)

size_t mbrtowc(wchar_t *pwc, const char *s, size_t n, mbstate_t *ps)
{
	static fn_t real;
	const unsigned char *u = (const unsigned char *)s;
	const char *m = mutant();

	if (real == NULL)
		real = (fn_t)dlsym(RTLD_NEXT, "mbrtowc");
	if (m != NULL && s != NULL) {
		if (IS("overlong") && n >= 2 && u[0] == 0xC0 && u[1] >= 0x80 && u[1] <= 0xBF) {
			if (pwc != NULL)
				*pwc = u[1] & 0x3F;
			return 2;
		}
		if (IS("surrogate") && n >= 3 && u[0] == 0xED && u[1] >= 0xA0 && u[1] <= 0xBF && u[2] >= 0x80 && u[2] <= 0xBF) {
			if (pwc != NULL)
				*pwc = 0xD000 | ((u[1] & 0x3Fu) << 6) | (u[2] & 0x3Fu);
			return 3;
		}
		if (IS("above") && n >= 4 && u[0] == 0xF4 && u[1] >= 0x90 && u[1] <= 0xBF) {
			if (pwc != NULL)
				*pwc = 0x100000 | ((u[1] & 0x3Fu) << 12) | ((u[2] & 0x3Fu) << 6) | (u[3] & 0x3Fu);
			return 4;
		}
		if (IS("truncated") && n >= 2 && u[0] == 0xE2 && u[1] == 0x82 && (n == 2 || u[2] < 0x80 || u[2] > 0xBF)) {
			if (pwc != NULL)
				*pwc = 0x2080;
			return 2;
		}
		if (IS("stray") && n >= 1 && u[0] >= 0x80 && u[0] <= 0xBF) {
			if (pwc != NULL)
				*pwc = u[0];
			return 1;
		}
	}
	return real(pwc, s, n, ps);
}
