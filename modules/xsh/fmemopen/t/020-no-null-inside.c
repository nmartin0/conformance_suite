/*
 * A write that does not move the end position adds no null byte.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.fmemopen.020
 * MUTANT    fmemopen.nul_always
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
	pc_begin("XSH.fmemopen.020");
	{
		static char b1[8], b2[6];
		char got[16];
		size_t n;
		FILE *f;

		memset(b1, 'x', sizeof b1);
		memcpy(b2, "abcdef", 6);
		pc_test();
		f = mem(b1, 8, "w+");
		NEED(fputs("abcdef", f) != EOF, "fputs(\"abcdef\") failed: %s", pc_errname(errno));
		NEED(fseek(f, 1, SEEK_SET) == 0, "fseek(1) failed: %s", pc_errname(errno));
		NEED(fputs("Z", f) != EOF, "fputs(\"Z\") failed: %s", pc_errname(errno));
		NEED(fflush(f) == 0, "fflush() failed: %s", pc_errname(errno));
		pc_check(b1[2] == 'c', "\"w+\": after a write at 1 the buffer holds \"%s\"", show(b1, 7));
		(void)fclose(f);
		f = mem(b2, 6, "r+");
		NEED(fputs("XY", f) != EOF, "fputs(\"XY\") failed: %s", pc_errname(errno));
		NEED(fflush(f) == 0, "fflush() failed: %s", pc_errname(errno));
		pc_check(b2[2] == 'c', "\"r+\": after a write at 0 the buffer holds \"%s\"", show(b2, 6));
		rewind(f);
		n = fread(got, 1, sizeof got, f);
		pc_check(n == 6 && memcmp(got, "XYcdef", 6) == 0, "\"r+\": reads gave %lu bytes \"%s\", expected \"XYcdef\"",
			 (unsigned long)n, show(got, n));
		(void)fclose(f);
	}
	pc_pass();
}
