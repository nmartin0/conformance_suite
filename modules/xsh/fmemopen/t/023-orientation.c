/*
 * Memory streams start with no orientation, as file streams do.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.fmemopen.023
 * MUTANT    fmemopen.byte_oriented
 * MUTANT    fmemopen.wide_after_read
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
	pc_begin("XSH.fmemopen.023");
	{
		static char b1[4] = { 'a', 'b', 'c', 'd' }, b2[4];
		FILE *f, *g;
		int o;

		pc_test();
		f = mem(b1, 4, "r");
		g = mem(b2, 4, "w+");
		o = fwide(f, 0);
		pc_check(o == 0, "\"r\": fwide(f, 0) right after fmemopen() returned %d, expected 0", o);
		o = fwide(g, 0);
		pc_check(o == 0, "\"w+\": fwide(f, 0) right after fmemopen() returned %d, expected 0", o);
		NEED(fgetc(f) == 'a', "the read did not give 'a'");
		o = fwide(f, 0);
		pc_check(o < 0, "\"r\": after fgetc() fwide(f, 0) returned %d, expected a negative value", o);
		(void)fclose(f);
		(void)fclose(g);
	}
	pc_pass();
}
