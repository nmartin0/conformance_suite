/*
 * A write past the end position moves it; a write inside does not.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.fmemopen.017
 * MUTANT    fmemopen.end_fixed
 * MUTANT    fmemopen.end_shrinks
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
	pc_begin("XSH.fmemopen.017");
	{
		static char buf[10];
		FILE *f;
		long p;

		memset(buf, 'x', sizeof buf);
		pc_test();
		f = mem(buf, sizeof buf, "w+");
		NEED(fputs("abc", f) != EOF, "fputs(\"abc\") failed: %s", pc_errname(errno));
		p = seek_to(f, 0, SEEK_END, "w+");
		pc_check(p == 3, "after writing 3 bytes the end position is %ld, expected 3", p);
		NEED(fseek(f, 0, SEEK_SET) == 0, "fseek(0) failed: %s", pc_errname(errno));
		NEED(fputs("Q", f) != EOF, "fputs(\"Q\") failed: %s", pc_errname(errno));
		p = seek_to(f, 0, SEEK_END, "w+");
		pc_check(p == 3, "after a write at 0 the end position is %ld, expected 3", p);
		NEED(fseek(f, 5, SEEK_SET) == 0, "fseek(5) failed: %s", pc_errname(errno));
		NEED(fputs("Z", f) != EOF, "fputs(\"Z\") failed: %s", pc_errname(errno));
		p = seek_to(f, 0, SEEK_END, "w+");
		pc_check(p == 6, "after a write at 5 the end position is %ld, expected 6", p);
		(void)fclose(f);
	}
	pc_pass();
}
