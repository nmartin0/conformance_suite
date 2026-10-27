/*
 * A matching failure ends the call; the result counts the items stored.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.fwscanf.006
 */
#include "pc.h"
#include <errno.h>
#include <fcntl.h>
#include <limits.h>
#include <locale.h>
#include <signal.h>
#include <stdarg.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>
#include <wchar.h>

/* After pc_test(): a step that is not under test gives UNRESOLVED, not FAIL. */
#define NEED(cond, ...) do { if (!(cond)) pc_unresolved(__VA_ARGS__); } while (0)

/* A stream on path in mode (a precondition). */
static FILE *open_as(const char *path, const char *mode)
{
	FILE *f = fopen(path, mode);

	NEED(f != NULL, "fopen(%s, %s) failed: %s", path, mode, pc_errname(errno));
	return f;
}

/* The ASCII string s as wide characters, in one of four rotating buffers. */
static const wchar_t *widen(const char *s)
{
	static wchar_t bufs[4][256];
	static int k;
	wchar_t *b = bufs[k++ & 3];
	size_t i;

	for (i = 0; s[i] != '\0' && i < 255; i++)
		b[i] = (wchar_t)(unsigned char)s[i];
	b[i] = L'\0';
	return b;
}

#define SCS swscanf
#define SCF fwscanf
#define SCW wscanf

/* A new file "in.txt" holding the ASCII text in, opened as a stream. */
static FILE *scan_stream(const char *in)
{
	int fd = open("in.txt", O_WRONLY | O_CREAT | O_TRUNC, 0666);
	size_t n = strlen(in);

	NEED(fd >= 0, "open(in.txt) failed: %s", pc_errname(errno));
	NEED(n == 0 || write(fd, in, n) == (ssize_t)n, "write(in.txt) failed: %s", pc_errname(errno));
	NEED(close(fd) == 0, "close(in.txt) failed: %s", pc_errname(errno));
	return open_as("in.txt", "r");
}

/* Makes standard input a new stream on a file holding the ASCII text in. */
static void scan_stdin(const char *in)
{
	FILE *f = scan_stream(in);

	(void)fclose(f);
	NEED(freopen("in.txt", "r", stdin) != NULL, "freopen(stdin) failed: %s", pc_errname(errno));
}

/*
 * SC(r, in, format, ...): the v-th scanner reads the ASCII text in, v being
 * 0 for the string form, 1 for the stream form (on the stream sf) and 2 for
 * the standard-input form; r receives the return value.
 */
#define SC(r, in, ...) do { \
	switch (v) { \
	case 0: r = SCS(widen(in), __VA_ARGS__); break; \
	case 1: sf = scan_stream(in); r = SCF(sf, __VA_ARGS__); break; \
	default: scan_stdin(in); r = SCW(__VA_ARGS__); break; \
	} \
} while (0)
static const char *const vname[3] = { "string form", "stream form", "standard-input form" };

int main(void)
{
	pc_begin("XSH.fwscanf.006");
	{
		int v, r;
		FILE *sf = NULL;
		int a, b;

		pc_test();
		for (v = 0; v < 3; v++) {
			a = b = 77;
			SC(r, "12 x", L"%d %d", &a, &b);
			pc_check(r == 1 && a == 12 && b == 77, "%s: \"%%d %%d\" on \"12 x\" returned %d, expected 1 with only the first stored", vname[v], r);
			a = 77;
			SC(r, "x", L"%d", &a);
			pc_check(r == 0 && a == 77, "%s: %%d on \"x\" returned %d, expected 0 with nothing stored", vname[v], r);
		}
		(void)sf;
	}
	pc_pass();
}
