/*
 * fflush() after a seek sets the size to the smaller of length and position.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.open_memstream.005
 * MUTANT    open_memstream.size_is_len
 * MUTANT    open_memstream.size_stale
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
	pc_begin("XSH.open_memstream.005");
	{
		char *b = NULL;
		size_t n = 0;
		FILE *f;

		pc_test();
		f = ms(&b, &n);
		NEED(fputs("hello", f) != EOF, "fputs() failed: %s", pc_errname(errno));
		NEED(fflush(f) == 0, "fflush() failed: %s", pc_errname(errno));
		NEED(n == 5, "the size after writing 5 bytes is %lu", (unsigned long)n);
		pc_check(fseek(f, 2, SEEK_SET) == 0, "fseek(2) failed: %s", pc_errname(errno));
		pc_check(fflush(f) == 0, "fflush() failed: %s", pc_errname(errno));
		pc_check(n == 2, "after fseek(2) and fflush() the size is %lu, expected 2", (unsigned long)n);
		pc_check(fseek(f, 4, SEEK_SET) == 0, "fseek(4) failed: %s", pc_errname(errno));
		pc_check(fflush(f) == 0, "fflush() failed: %s", pc_errname(errno));
		pc_check(n == 4, "after fseek(4) and fflush() the size is %lu, expected 4", (unsigned long)n);
		(void)fclose(f);
		free(b);
	}
	pc_pass();
}
