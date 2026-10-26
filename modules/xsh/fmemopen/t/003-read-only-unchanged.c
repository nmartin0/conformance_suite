/*
 * Operations on a read-only stream leave the caller's buffer unchanged.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.fmemopen.003
 * MUTANT    fmemopen.r_accepts_writes
 * MUTANT    fmemopen.r_terminates
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
	pc_begin("XSH.fmemopen.003");
	{
		static char buf[6] = { 'a', 'b', 'c', 'd', 'e', 'f' };
		FILE *f;
		int c;

		pc_test();
		f = mem(buf, sizeof buf, "r");
		NEED(fgetc(f) == 'a', "the first read did not give 'a'");
		NEED(ungetc('Z', f) == 'Z', "ungetc('Z') failed");
		NEED(fgetc(f) == 'Z', "the pushed-back byte was not read back");
		NEED(fseek(f, 1, SEEK_SET) == 0, "fseek(1) failed: %s", pc_errname(errno));
		(void)fputc('Q', f);	/* not judged: only its effect on the buffer is */
		(void)fflush(f);
		clearerr(f);
		NEED(fseek(f, 3, SEEK_SET) == 0, "fseek(3) failed: %s", pc_errname(errno));
		c = fgetc(f);
		NEED(c == 'd', "the read at 3 gave %d, expected 'd'", c);
		(void)fflush(f);
		NEED(fseek(f, 2, SEEK_SET) == 0, "fseek(2) failed: %s", pc_errname(errno));
		(void)fclose(f);
		pc_check(memcmp(buf, "abcdef", 6) == 0, "the buffer now holds \"%s\"", show(buf, 6));
	}
	pc_pass();
}
