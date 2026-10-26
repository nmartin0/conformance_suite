/*
 * The buffer grows to hold everything written.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.open_memstream.003
 * MUTANT    open_memstream.fixed_cap
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
	pc_begin("XSH.open_memstream.003");
	{
		char piece[101], *b = NULL;
		size_t n = 0, i;
		FILE *f;

		pc_test();
		f = ms(&b, &n);
		for (i = 0; i < 1000; i++) {
			snprintf(piece, sizeof piece, "%0100lu", (unsigned long)i);
			pc_check(fputs(piece, f) != EOF, "fputs() of piece %lu failed: %s", (unsigned long)i, pc_errname(errno));
		}
		pc_check(fclose(f) == 0, "fclose() failed: %s", pc_errname(errno));
		pc_check(n == 100000, "after fclose() the size is %lu, expected 100000", (unsigned long)n);
		pc_check(b != NULL, "after fclose() the buffer pointer is null");
		for (i = 0; i < 1000; i++) {
			snprintf(piece, sizeof piece, "%0100lu", (unsigned long)i);
			pc_check(memcmp(b + i * 100, piece, 100) == 0, "piece %lu reads back as \"%s\"", (unsigned long)i,
				 show(b + i * 100, 100));
		}
		pc_check(b[100000] == '\0', "byte 100000 is %#x, expected a null byte", (unsigned)(unsigned char)b[100000]);
		free(b);
	}
	pc_pass();
}
