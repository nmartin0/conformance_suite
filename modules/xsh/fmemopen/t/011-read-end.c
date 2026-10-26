/*
 * For 'r' modes the end position is max_size and does not move.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.fmemopen.011
 * MUTANT    fmemopen.r_end_strlen
 * MUTANT    fmemopen.r_end_follows
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
	pc_begin("XSH.fmemopen.011");
	{
		static char b1[6], b2[6];
		FILE *f;
		long p;

		memcpy(b1, "ab\0def", 6);
		memcpy(b2, "ab\0def", 6);
		pc_test();
		f = mem(b1, 6, "r");
		p = seek_to(f, 0, SEEK_END, "r");
		pc_check(p == 6, "\"r\": the end position is %ld, expected 6", p);
		(void)fclose(f);
		f = mem(b2, 6, "r+");
		p = seek_to(f, 0, SEEK_END, "r+");
		pc_check(p == 6, "\"r+\": the end position is %ld, expected 6", p);
		rewind(f);
		NEED(fputs("XY", f) != EOF, "fputs(\"XY\") failed: %s", pc_errname(errno));
		p = seek_to(f, 0, SEEK_END, "r+");
		pc_check(p == 6, "\"r+\": after a write at 0 the end position is %ld, expected 6", p);
		NEED(fseek(f, 4, SEEK_SET) == 0, "fseek(4) failed: %s", pc_errname(errno));
		NEED(fputc('Q', f) == 'Q', "fputc('Q') failed: %s", pc_errname(errno));
		p = seek_to(f, 0, SEEK_END, "r+");
		pc_check(p == 6, "\"r+\": after a write at 4 the end position is %ld, expected 6", p);
		(void)fclose(f);
	}
	pc_pass();
}
