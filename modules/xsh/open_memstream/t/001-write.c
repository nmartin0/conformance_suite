/*
 * Data written to a dynamic memory stream appears in its buffer.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.open_memstream.001
 * MUTANT    open_memstream.size_counts_nul
 * MUTANT    open_memstream.no_nul
 * MUTANT    open_memstream.pos_stuck
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
	pc_begin("XSH.open_memstream.001");
	{
		char *b = NULL;
		size_t n = 99;
		FILE *f;
		long p;

		pc_test();
		f = ms(&b, &n);
		p = ftell(f);
		pc_check(p == 0, "the initial position is %ld, expected 0", p);
		NEED(fputs("hello", f) != EOF, "fputs(\"hello\") failed: %s", pc_errname(errno));
		p = ftell(f);
		pc_check(p == 5, "after writing 5 bytes the position is %ld, expected 5", p);
		pc_check(fflush(f) == 0, "fflush() failed: %s", pc_errname(errno));
		pc_check(n == 5, "after fflush() the size is %lu, expected 5", (unsigned long)n);
		pc_check(b != NULL && memcmp(b, "hello", 6) == 0, "after fflush() the buffer holds \"%s\"",
			 b != NULL ? show(b, 6) : "(null)");
		pc_check(fclose(f) == 0, "fclose() failed: %s", pc_errname(errno));
		pc_check(n == 5, "after fclose() the size is %lu, expected 5", (unsigned long)n);
		pc_check(b != NULL && memcmp(b, "hello", 6) == 0, "after fclose() the buffer holds \"%s\"",
			 b != NULL ? show(b, 6) : "(null)");
		free(b);
	}
	pc_pass();
}
