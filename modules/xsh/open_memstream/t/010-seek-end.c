/*
 * SEEK_END is relative to the length or to the size fflush() would set.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.open_memstream.010
 * MUTANT    open_memstream.seek_end_zero
 * MUTANT    open_memstream.seek_end_cap
 * VARIANT   open_memstream.unbuffered
 * VARIANT   open_memstream.seek_end_flush_size
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
	pc_begin("XSH.open_memstream.010");
	{
		char *b = NULL;
		size_t n = 0;
		FILE *f;
		long p;

		pc_test();
		f = ms(&b, &n);
		NEED(fputs("hello", f) != EOF, "fputs() failed: %s", pc_errname(errno));
		NEED(fseek(f, 2, SEEK_SET) == 0, "fseek(2) failed: %s", pc_errname(errno));
		pc_check(fseek(f, 0, SEEK_END) == 0, "fseek(0, SEEK_END) failed: %s", pc_errname(errno));
		p = ftell(f);
		pc_check(p == 5 || p == 2, "after fseek(0, SEEK_END) the position is %ld, expected 5 or 2", p);
		pc_note("SEEK_END is relative to %s", p == 5 ? "the length" : "the size fflush() would set");
		(void)fclose(f);
		free(b);
	}
	pc_pass();
}
