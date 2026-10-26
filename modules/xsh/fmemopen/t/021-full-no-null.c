/*
 * A write that fills the buffer exactly keeps every byte and adds no null byte.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.fmemopen.021
 * MUTANT    fmemopen.nul_in_last
 * MUTANT    fmemopen.nul_overrun
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
	pc_begin("XSH.fmemopen.021");
	{
		static char b1[8], b2[8], b3[8];
		char got[8];
		size_t n;
		FILE *f;
		int i;

		memset(b1, 'x', sizeof b1);
		memcpy(b2, "ab\0\0xxxx", 8);
		memset(b3, 'x', sizeof b3);
		pc_test();
		f = mem(b1, 4, "w");
		pc_check(fputs("abcd", f) != EOF, "\"w\": fputs(\"abcd\") failed: %s", pc_errname(errno));
		pc_check(fflush(f) == 0, "\"w\": fflush() of 4 bytes into 4 failed: %s", pc_errname(errno));
		pc_check(memcmp(b1, "abcd", 4) == 0, "\"w\": the buffer holds \"%s\", expected \"abcd\"", show(b1, 4));
		for (i = 4; i < 8; i++)
			pc_check(b1[i] == 'x', "\"w\": byte %d, outside the buffer, became %#x", i, (unsigned)(unsigned char)b1[i]);
		(void)fclose(f);
		f = mem(b2, 4, "a");
		pc_check(fputs("cd", f) != EOF, "\"a\": fputs(\"cd\") failed: %s", pc_errname(errno));
		pc_check(fflush(f) == 0, "\"a\": fflush() of 2 bytes after 2 failed: %s", pc_errname(errno));
		pc_check(memcmp(b2, "abcd", 4) == 0, "\"a\": the buffer holds \"%s\", expected \"abcd\"", show(b2, 4));
		for (i = 4; i < 8; i++)
			pc_check(b2[i] == 'x', "\"a\": byte %d, outside the buffer, became %#x", i, (unsigned)(unsigned char)b2[i]);
		(void)fclose(f);
		f = mem(b3, 4, "w+");
		pc_check(fputs("abcd", f) != EOF, "\"w+\": fputs(\"abcd\") failed: %s", pc_errname(errno));
		pc_check(fflush(f) == 0, "\"w+\": fflush() of 4 bytes into 4 failed: %s", pc_errname(errno));
		pc_check(memcmp(b3, "abcd", 4) == 0, "\"w+\": the buffer holds \"%s\", expected \"abcd\"", show(b3, 4));
		rewind(f);
		n = fread(got, 1, sizeof got, f);
		pc_check(n == 4 && memcmp(got, "abcd", 4) == 0, "\"w+\": reads gave %lu bytes \"%s\"", (unsigned long)n,
			 show(got, n));
		for (i = 4; i < 8; i++)
			pc_check(b3[i] == 'x', "\"w+\": byte %d, outside the buffer, became %#x", i, (unsigned)(unsigned char)b3[i]);
		(void)fclose(f);
	}
	pc_pass();
}
