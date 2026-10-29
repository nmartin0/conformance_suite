/*
 * wcrtomb.c - planted faults in wcrtomb(), for the UTF-8 checks (CI only).
 *
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * A preload library; faults are named "wcrtomb.<fault>" and apply only while
 * PC_PHASE is "test".
 */
#define _GNU_SOURCE 1
#include <dlfcn.h>
#include <stdlib.h>
#include <string.h>
#include <wchar.h>

typedef size_t (*fn_t)(char *, wchar_t, mbstate_t *);

static const char *mutant(void)
{
	const char *ph = getenv("PC_PHASE");
	const char *m = getenv("PC_MUTANT");

	if (ph == NULL || strcmp(ph, "test") != 0 || m == NULL || strncmp(m, "wcrtomb.", 8) != 0)
		return NULL;
	return m + 8;
}

#define IS(name) (m != NULL && strcmp(m, name) == 0)

size_t wcrtomb(char *s, wchar_t wc, mbstate_t *ps)
{
	static fn_t real;
	const char *m = mutant();
	unsigned long v = (unsigned long)wc;

	if (real == NULL)
		real = (fn_t)dlsym(RTLD_NEXT, "wcrtomb");
	if (m != NULL && s != NULL) {
		if (IS("long") && v >= 0x80 && v < 0x800) {	/* three octets for a two-octet number */
			s[0] = (char)(0xE0 | (v >> 12));
			s[1] = (char)(0x80 | ((v >> 6) & 0x3F));
			s[2] = (char)(0x80 | (v & 0x3F));
			return 3;
		}
		if (IS("surrogate") && v >= 0xD800 && v <= 0xDFFF) {
			s[0] = (char)(0xE0 | (v >> 12));
			s[1] = (char)(0x80 | ((v >> 6) & 0x3F));
			s[2] = (char)(0x80 | (v & 0x3F));
			return 3;
		}
		if (IS("above") && v >= 0x110000 && v < 0x200000) {
			s[0] = (char)(0xF0 | (v >> 18));
			s[1] = (char)(0x80 | ((v >> 12) & 0x3F));
			s[2] = (char)(0x80 | ((v >> 6) & 0x3F));
			s[3] = (char)(0x80 | (v & 0x3F));
			return 4;
		}
	}
	return real(s, wc, ps);
}
