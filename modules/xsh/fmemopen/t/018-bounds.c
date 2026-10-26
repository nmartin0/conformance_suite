/*
 * Writes that do not fit stay within max_size bytes.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.fmemopen.018
 * MUTANT    fmemopen.overrun
 * MUTANT    fmemopen.nul_overrun
 * MUTANT    fmemopen.end_unbounded
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
	pc_begin("XSH.fmemopen.018");
	{
		static char a1[12], a2[12];
		FILE *f;
		long p;
		int i;

		memset(a1, 'x', sizeof a1);
		memset(a2, 'x', sizeof a2);
		pc_test();
		f = mem(a1, 4, "w");
		(void)fputs("abcdefgh", f);	/* how much fits, and the errors, are not judged */
		(void)fclose(f);
		for (i = 4; i < 12; i++)
			pc_check(a1[i] == 'x', "\"w\": byte %d, outside the 4-byte buffer, became %#x", i,
				 (unsigned)(unsigned char)a1[i]);
		f = mem(a2, 4, "w+");
		NEED(setvbuf(f, NULL, _IONBF, 0) == 0, "setvbuf(_IONBF) failed");
		(void)fputs("abcdefgh", f);
		for (i = 4; i < 12; i++)
			pc_check(a2[i] == 'x', "\"w+\" unbuffered: byte %d, outside the 4-byte buffer, became %#x", i,
				 (unsigned)(unsigned char)a2[i]);
		clearerr(f);
		NEED(fseek(f, 0, SEEK_END) == 0, "fseek(0, SEEK_END) failed: %s", pc_errname(errno));
		p = ftell(f);
		pc_check(p >= 0 && p <= 4, "\"w+\" unbuffered: the end position is %ld, beyond max_size 4", p);
		(void)fclose(f);
		for (i = 4; i < 12; i++)
			pc_check(a2[i] == 'x', "\"w+\": after fclose() byte %d became %#x", i, (unsigned)(unsigned char)a2[i]);
	}
	pc_pass();
}
