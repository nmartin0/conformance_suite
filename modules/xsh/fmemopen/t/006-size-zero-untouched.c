/*
 * With max_size 0, fmemopen() does not write at buf.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.fmemopen.006
 * MUTANT    fmemopen.zero_nul
 * MUTANT    fmemopen.zero_wrong_errno
 * VARIANT   fmemopen.size0_einval
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
	pc_begin("XSH.fmemopen.006");
	{
		static const char *modes[2] = { "w", "w+" };
		static char byte[2][1] = { { 'x' }, { 'x' } };
		FILE *f;
		int i, e;

		pc_test();
		for (i = 0; i < 2; i++) {
			errno = 0;
			f = fmemopen(byte[i], 0, modes[i]);
			e = errno;
			pc_check(byte[i][0] == 'x', "fmemopen(buf, 0, \"%s\") changed the byte at buf to %#x", modes[i],
				 (unsigned)(unsigned char)byte[i][0]);
			if (f == NULL) {
				/* A size of zero may be unsupported. */
				pc_check(e == EINVAL, "fmemopen(buf, 0, \"%s\") failed with %s, expected EINVAL", modes[i],
					 pc_errname(e));
				pc_note("fmemopen(buf, 0, \"%s\") failed with EINVAL (size zero unsupported)", modes[i]);
				continue;
			}
			(void)fclose(f);
			pc_check(byte[i][0] == 'x', "after fclose() of the \"%s\" stream the byte at buf is %#x", modes[i],
				 (unsigned)(unsigned char)byte[i][0]);
		}
	}
	pc_pass();
}
