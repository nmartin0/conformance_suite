/*
 * Writes on non-append streams start at the current position.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.fmemopen.015
 * MUTANT    fmemopen.write_at_end
 * MUTANT    fmemopen.write_at_start
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
	pc_begin("XSH.fmemopen.015");
	{
		static char b1[6], b2[8];
		char got[16];
		size_t n;
		FILE *f;
		long p;

		memcpy(b1, "abcdef", 6);
		pc_test();
		f = mem(b1, 6, "r+");
		NEED(fseek(f, 2, SEEK_SET) == 0, "fseek(2) failed: %s", pc_errname(errno));
		pc_check(fputs("XY", f) != EOF, "\"r+\": fputs(\"XY\") failed: %s", pc_errname(errno));
		p = ftell(f);
		pc_check(p == 4, "\"r+\": after the write at 2 the position is %ld, expected 4", p);
		pc_check(fflush(f) == 0, "\"r+\": fflush() failed: %s", pc_errname(errno));
		pc_check(memcmp(b1, "abXYef", 6) == 0, "\"r+\": the buffer holds \"%s\", expected \"abXYef\"", show(b1, 6));
		(void)fclose(f);
		f = mem(b2, 8, "w+");
		NEED(fputs("abcdef", f) != EOF, "fputs(\"abcdef\") failed: %s", pc_errname(errno));
		NEED(fseek(f, 1, SEEK_SET) == 0, "fseek(1) failed: %s", pc_errname(errno));
		pc_check(fputs("Z", f) != EOF, "\"w+\": fputs(\"Z\") failed: %s", pc_errname(errno));
		rewind(f);
		n = fread(got, 1, sizeof got, f);
		pc_check(n == 6 && memcmp(got, "aZcdef", 6) == 0, "\"w+\": reads gave %lu bytes \"%s\", expected \"aZcdef\"",
			 (unsigned long)n, show(got, n));
		(void)fclose(f);
	}
	pc_pass();
}
