/*
 * Wide characters written to open_wmemstream() appear in its buffer, counted in characters.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.open_memstream.013
 * MUTANT    open_memstream.wsize_bytes
 * MUTANT    open_memstream.wno_nul
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
	pc_begin("XSH.open_memstream.013");
	{
		wchar_t *b = NULL;
		size_t n = 99;
		FILE *f;

		pc_test();
		errno = 0;
		f = open_wmemstream(&b, &n);
		pc_check(f != NULL, "open_wmemstream() failed: %s", pc_errname(errno));
		NEED(fputws(L"hello", f) != -1, "fputws() failed: %s", pc_errname(errno));
		pc_check(fflush(f) == 0, "fflush() failed: %s", pc_errname(errno));
		pc_check(n == 5, "after fflush() the size is %lu, expected 5 wide characters", (unsigned long)n);
		pc_check(b != NULL && wmemcmp(b, L"hello", 6) == 0, "after fflush() the buffer does not hold L\"hello\" and a null wide character");
		pc_check(fclose(f) == 0, "fclose() failed: %s", pc_errname(errno));
		pc_check(n == 5, "after fclose() the size is %lu, expected 5", (unsigned long)n);
		pc_check(b != NULL && wmemcmp(b, L"hello", 6) == 0, "after fclose() the buffer does not hold L\"hello\" and a null wide character");
		free(b);
	}
	pc_pass();
}
