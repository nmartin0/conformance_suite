/*
 * fgetws() reads up to and including a newline and terminates the string.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.fgetws.001
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

/* 1 if the wide string w holds exactly the ASCII characters of s. */
static int weq(const wchar_t *w, const char *s)
{
	size_t i;

	for (i = 0; s[i] != '\0'; i++)
		if (w[i] != (wchar_t)(unsigned char)s[i])
			return 0;
	return w[i] == L'\0';
}

/* A wide string as ASCII for messages ('?' for anything else). */
static const char *narrow(const wchar_t *w)
{
	static char bufs[4][128];
	static int k;
	char *b = bufs[k++ & 3];
	size_t i;

	for (i = 0; w[i] != L'\0' && i < 120; i++)
		b[i] = w[i] > 31 && w[i] < 127 ? (char)w[i] : '?';
	b[i] = '\0';
	return b;
}

int main(void)
{
	pc_begin("XSH.fgetws.001");
	{
		wchar_t ws[16];
		wchar_t *r;
		FILE *f;

		pc_make_file("f", "ab\ncd\n");
		pc_test();
		f = open_as("f", "r");
		wmemset(ws, L'#', 16);
		r = fgetws(ws, 10, f);
		pc_check(r == ws, "fgetws() returned %p, expected the array %p", (void *)r, (void *)ws);
		pc_check(wcscmp(ws, L"ab\n") == 0, "the first line is \"%s\", expected \"ab\\n\"", narrow(ws));
		wmemset(ws, L'#', 16);
		r = fgetws(ws, 10, f);
		pc_check(r == ws, "the second fgetws() returned %p, expected the array", (void *)r);
		pc_check(wcscmp(ws, L"cd\n") == 0, "the second line is \"%s\", expected \"cd\\n\"", narrow(ws));
	}
	pc_pass();
}
