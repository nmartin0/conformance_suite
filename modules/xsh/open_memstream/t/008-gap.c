/*
 * Bytes skipped by a seek past the length read as null bytes after a later write.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.open_memstream.008
 * MUTANT    open_memstream.gap_garbage
 * VARIANT   open_memstream.unbuffered
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
	pc_begin("XSH.open_memstream.008");
	{
		char *b = NULL;
		size_t n = 0;
		FILE *f;

		pc_test();
		f = ms(&b, &n);
		NEED(fputs("ab", f) != EOF, "fputs() failed: %s", pc_errname(errno));
		pc_check(fseek(f, 5, SEEK_SET) == 0, "fseek(5) past the length failed: %s", pc_errname(errno));
		NEED(fputs("c", f) != EOF, "fputs(\"c\") failed: %s", pc_errname(errno));
		pc_check(fflush(f) == 0, "fflush() failed: %s", pc_errname(errno));
		pc_check(n == 6, "the size is %lu, expected 6", (unsigned long)n);
		pc_check(b != NULL && memcmp(b, "ab\0\0\0c", 7) == 0, "the buffer holds \"%s\"",
			 b != NULL ? show(b, 7) : "(null)");
		(void)fclose(f);
		free(b);
	}
	pc_pass();
}
