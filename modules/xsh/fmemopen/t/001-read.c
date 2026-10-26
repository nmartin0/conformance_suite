/*
 * A read stream over a buffer gives its bytes and then end-of-file.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.fmemopen.001
 * MUTANT    fmemopen.read_skip
 * MUTANT    fmemopen.eof_early
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
	pc_begin("XSH.fmemopen.001");
	{
		static char buf[6] = { 'f', 'o', 'o', 'b', 'a', 'r' };
		const char *want = "foobar";
		FILE *f;
		int i, c;

		pc_test();
		f = mem(buf, sizeof buf, "r");
		for (i = 0; i < 6; i++) {
			c = fgetc(f);
			pc_check(c == (unsigned char)want[i], "read %d gave %d, expected '%c'", i, c, want[i]);
		}
		c = fgetc(f);
		pc_check(c == EOF, "a seventh read gave %d, expected EOF", c);
		pc_check(feof(f) != 0, "the end-of-file indicator is clear after reading to the end");
		pc_check(ferror(f) == 0, "the error indicator is set after reading to the end");
		(void)fclose(f);
	}
	pc_pass();
}
