/*
 * Seeks within max_size succeed; seeks before 0 or past max_size fail.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.fmemopen.022
 * MUTANT    fmemopen.seek_unbounded
 * MUTANT    fmemopen.seek_past_end_fails
 * VARIANT   setvbuf.tiny_default
 * VARIANT   fmemopen.unbuffered
 * VARIANT   fseek.mem_fail_clamps
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
	pc_begin("XSH.fmemopen.022");
	{
		static char buf[10];
		FILE *f;
		long p;

		pc_test();
		f = mem(buf, sizeof buf, "w+");
		NEED(fputs("abc", f) != EOF, "fputs(\"abc\") failed: %s", pc_errname(errno));
		pc_check(fseek(f, 7, SEEK_SET) == 0, "fseek(7, SEEK_SET), past the end position, failed: %s", pc_errname(errno));
		pc_check(fseek(f, 10, SEEK_SET) == 0, "fseek(10, SEEK_SET), at max_size, failed: %s", pc_errname(errno));
		pc_check(fseek(f, 7, SEEK_END) == 0, "fseek(7, SEEK_END), to 10, failed: %s", pc_errname(errno));
		pc_check(fseek(f, 11, SEEK_SET) == -1, "fseek(11, SEEK_SET), past max_size, succeeded");
		pc_check(fseek(f, -1, SEEK_SET) == -1, "fseek(-1, SEEK_SET) succeeded");
		NEED(fseek(f, 0, SEEK_SET) == 0, "fseek(0) failed: %s", pc_errname(errno));
		pc_check(fseek(f, 11, SEEK_CUR) == -1, "fseek(11, SEEK_CUR) from 0 succeeded");
		pc_check(fseek(f, 8, SEEK_END) == -1, "fseek(8, SEEK_END), to 11, succeeded");
		p = ftell(f);
		pc_check(p >= 0 && p <= 10, "after the failed seeks ftell() is %ld, outside the buffer", p);
		(void)fclose(f);
	}
	pc_pass();
}
