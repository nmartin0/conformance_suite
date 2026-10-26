/*
 * Writes on append streams start at the end position.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.fmemopen.016
 * MUTANT    fmemopen.a_write_at_pos
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
	pc_begin("XSH.fmemopen.016");
	{
		static char b1[8] = { 'a', 'b' }, b2[8] = { 'a', 'b' };
		FILE *f;

		pc_test();
		f = mem(b1, 8, "a+");
		NEED(fseek(f, 0, SEEK_SET) == 0, "fseek(0) failed: %s", pc_errname(errno));
		NEED(fgetc(f) == 'a', "the read at 0 did not give 'a'");
		NEED(fseek(f, 0, SEEK_SET) == 0, "fseek(0) failed: %s", pc_errname(errno));
		pc_check(fputs("XY", f) != EOF, "\"a+\": fputs(\"XY\") failed: %s", pc_errname(errno));
		pc_check(fflush(f) == 0, "fflush() failed: %s", pc_errname(errno));
		pc_check(memcmp(b1, "abXY", 4) == 0, "\"a+\": the buffer starts \"%s\", expected \"abXY\"", show(b1, 4));
		(void)fclose(f);
		f = mem(b2, 8, "a");
		NEED(fseek(f, 0, SEEK_SET) == 0, "fseek(0) failed: %s", pc_errname(errno));
		pc_check(fputs("Q", f) != EOF, "\"a\": fputs(\"Q\") failed: %s", pc_errname(errno));
		pc_check(fflush(f) == 0, "fflush() failed: %s", pc_errname(errno));
		pc_check(memcmp(b2, "abQ", 3) == 0, "\"a\": the buffer starts \"%s\", expected \"abQ\"", show(b2, 3));
		(void)fclose(f);
	}
	pc_pass();
}
