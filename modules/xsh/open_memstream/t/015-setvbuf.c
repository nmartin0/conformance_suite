/*
 * A dynamic memory stream works with a buffer from setvbuf() and unbuffered.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.open_memstream.015
 * MUTANT    open_memstream.setvbuf_loses
 */
#include "pc.h"
#include <errno.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/resource.h>
#include <sys/types.h>
#include <unistd.h>
#include <wchar.h>

/* After pc_test(): a step that is not under test gives UNRESOLVED, not FAIL. */
#define NEED(cond, ...) do { if (!(cond)) pc_unresolved(__VA_ARGS__); } while (0)

/* A dynamic memory stream, which must open (the call under test). */
static FILE *ms(char **b, size_t *n)
{
	FILE *f;

	errno = 0;
	f = open_memstream(b, n);
	pc_check(f != NULL, "open_memstream() failed: %s", pc_errname(errno));
	return f;
}

/* Bytes as text for messages (printable or \ooo). */
static const char *show(const void *p, size_t n)
{
	static char s[256];
	const unsigned char *b = p;
	size_t i, k = 0;

	for (i = 0; i < n && k + 5 < sizeof s; i++)
		k += (size_t)snprintf(s + k, sizeof s - k, b[i] >= 0x20 && b[i] < 0x7f ? "%c" : "\\%03o", b[i]);
	s[k] = '\0';
	return s;
}

int main(void)
{
	pc_begin("XSH.open_memstream.015");
	{
		static char sbuf[64];
		static const int types[2] = { _IOFBF, _IONBF };
		char *b;
		size_t n;
		FILE *f;
		int i;

		pc_test();
		for (i = 0; i < 2; i++) {
			b = NULL;
			n = 0;
			f = ms(&b, &n);
			NEED(setvbuf(f, i == 0 ? sbuf : NULL, types[i], i == 0 ? sizeof sbuf : 0) == 0,
			     "setvbuf(%s) failed", i == 0 ? "_IOFBF" : "_IONBF");
			pc_check(fputs("hello", f) != EOF, "%s: fputs() failed: %s", i == 0 ? "_IOFBF" : "_IONBF",
				 pc_errname(errno));
			pc_check(fflush(f) == 0, "%s: fflush() failed: %s", i == 0 ? "_IOFBF" : "_IONBF", pc_errname(errno));
			pc_check(n == 5, "%s: the size is %lu, expected 5", i == 0 ? "_IOFBF" : "_IONBF", (unsigned long)n);
			pc_check(b != NULL && memcmp(b, "hello", 6) == 0, "%s: the buffer holds \"%s\"",
				 i == 0 ? "_IOFBF" : "_IONBF", b != NULL ? show(b, 6) : "(null)");
			(void)fclose(f);
			free(b);
		}
	}
	pc_pass();
}
