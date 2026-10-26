/*
 * With a null buf, an append stream starts at 0 with end position 0.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.fmemopen.008
 * MUTANT    fmemopen.null_append_garbage
 * VARIANT   setvbuf.tiny_default
 * VARIANT   fmemopen.unbuffered
 */
#include "pc.h"
#include <errno.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/resource.h>
#include <unistd.h>
#include <wchar.h>

/* After pc_test(): a step that is not under test gives UNRESOLVED, not FAIL. */
#define NEED(cond, ...) do { if (!(cond)) pc_unresolved(__VA_ARGS__); } while (0)

/* A memory stream, which must open (the call under test). */
static FILE *mem(void *buf, size_t size, const char *mode)
{
	FILE *f;

	errno = 0;
	f = fmemopen(buf, size, mode);
	pc_check(f != NULL, "fmemopen(buf, %lu, \"%s\") failed: %s", (unsigned long)size, mode, pc_errname(errno));
	return f;
}

/* The position after fseek(f, off, whence), which must succeed. */
static long seek_to(FILE *f, long off, int whence, const char *mode)
{
	pc_check(fseek(f, off, whence) == 0, "\"%s\": fseek(%ld, %d) failed: %s", mode, off, whence, pc_errname(errno));
	return ftell(f);
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
	pc_begin("XSH.fmemopen.008");
	{
		char got[16];
		size_t n;
		FILE *f;
		long p;

		pc_test();
		errno = 0;
		f = fmemopen(NULL, 8, "a+");
		if (f == NULL && errno == ENOMEM)
			pc_unresolved("fmemopen(NULL, 8, \"a+\") could not allocate (ENOMEM)");
		pc_check(f != NULL, "fmemopen(NULL, 8, \"a+\") failed: %s", pc_errname(errno));
		p = ftell(f);
		pc_check(p == 0, "the initial position is %ld, expected 0", p);
		p = seek_to(f, 0, SEEK_END, "a+");
		pc_check(p == 0, "the initial end position is %ld, expected 0", p);
		pc_check(fputs("ab", f) != EOF, "fputs(\"ab\") failed: %s", pc_errname(errno));
		rewind(f);
		n = fread(got, 1, sizeof got, f);
		pc_check(n == 2 && memcmp(got, "ab", 2) == 0, "reads gave %lu bytes \"%s\", expected \"ab\"",
			 (unsigned long)n, show(got, n));
		(void)fclose(f);
	}
	pc_pass();
}
