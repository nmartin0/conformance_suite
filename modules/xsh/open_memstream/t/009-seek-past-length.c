/*
 * A seek past the length either extends the buffer with null bytes or not.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.open_memstream.009
 * MUTANT    open_memstream.extends_garbage
 * MUTANT    open_memstream.seek_past_fails
 * VARIANT   open_memstream.unbuffered
 * VARIANT   open_memstream.seek_extends
 * VARIANT   open_memstream.seek_keeps
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
	pc_begin("XSH.open_memstream.009");
	{
		char *b = NULL;
		size_t n = 0;
		FILE *f;

		pc_test();
		f = ms(&b, &n);
		NEED(fputs("ab", f) != EOF, "fputs() failed: %s", pc_errname(errno));
		pc_check(fseek(f, 5, SEEK_SET) == 0, "fseek(5) past the length failed: %s", pc_errname(errno));
		pc_check(fflush(f) == 0, "fflush() failed: %s", pc_errname(errno));
		pc_check(n == 2 || n == 5, "after the seek and fflush() the size is %lu, expected 2 or 5", (unsigned long)n);
		pc_check(b != NULL && memcmp(b, "ab", 2) == 0, "the buffer holds \"%s\"", b != NULL ? show(b, 2) : "(null)");
		if (n == 5)
			pc_check(memcmp(b + 2, "\0\0\0", 3) == 0, "the buffer was extended with \"%s\", expected null bytes",
				 show(b + 2, 3));
		pc_note("the size after the seek is %lu (%s)", (unsigned long)n, n == 5 ? "extended" : "not extended");
		(void)fclose(f);
		free(b);
	}
	pc_pass();
}
