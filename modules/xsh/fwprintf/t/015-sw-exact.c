/*
 * Output of n - 1 characters fits; n characters do not.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.fwprintf.015
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
	pc_begin("XSH.fwprintf.015");
	{
		wchar_t buf[8];
		int r;

		wmemset(buf, L'#', 8);
		pc_test();
		r = swprintf(buf, 4, L"abc");
		pc_check(r == 3, "swprintf(n 4) of three characters returned %d, expected 3", r);
		pc_check(weq(buf, "abc"), "the array holds \"%s\", expected \"abc\"", narrow(buf));
		r = swprintf(buf, 3, L"abc");
		pc_check(r < 0, "swprintf(n 3) of three characters returned %d, expected a negative value", r);
	}
	pc_pass();
}
