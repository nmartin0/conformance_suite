/*
 * The terminator counts toward maxsize and nothing is written beyond it.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.wcsftime.005
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

/* Fills tm for the given calendar date and time (year as tm_year). */
static void when(struct tm *tm, int year, int mon, int mday, int wday, int yday, int hour, int min, int sec)
{
	memset(tm, 0, sizeof *tm);
	tm->tm_year = year;
	tm->tm_mon = mon;
	tm->tm_mday = mday;
	tm->tm_wday = wday;
	tm->tm_yday = yday;
	tm->tm_hour = hour;
	tm->tm_min = min;
	tm->tm_sec = sec;
}

/* wcsftime() with fmt must give exactly the ASCII text want and return its length. */
static void chk(const struct tm *tm, const wchar_t *fmt, const char *want)
{
	wchar_t buf[128];
	size_t r, n = strlen(want);

	wmemset(buf, L'#', 128);
	r = wcsftime(buf, 128, fmt, tm);
	pc_check(r == n, "wcsftime(\"%s\") returned %lu, expected %lu", narrow(fmt), (unsigned long)r, (unsigned long)n);
	pc_check(weq(buf, want), "wcsftime(\"%s\") produced \"%s\", expected \"%s\"", narrow(fmt), narrow(buf), want);
}

int main(void)
{
	pc_begin("XSH.wcsftime.005");
	{
		wchar_t buf[32];
		struct tm tm;
		size_t r;
		int i;

		when(&tm, 100, 0, 2, 0, 1, 0, 0, 0);
		pc_test();
		wmemset(buf, L'#', 32);
		r = wcsftime(buf, 11, L"%F", &tm);
		pc_check(r == 10, "wcsftime(maxsize 11) returned %lu, expected 10", (unsigned long)r);
		pc_check(weq(buf, "2000-01-02"), "the array holds \"%s\", expected \"2000-01-02\" with its terminator", narrow(buf));
		for (i = 11; i < 32; i++)
			pc_check(buf[i] == L'#', "element %d, beyond maxsize, was changed", i);
		wmemset(buf, L'#', 32);
		r = wcsftime(buf, 10, L"%F", &tm);
		pc_check(r == 0, "wcsftime(maxsize 10) returned %lu, expected 0 (the result with its terminator needs 11)", (unsigned long)r);
		for (i = 10; i < 32; i++)
			pc_check(buf[i] == L'#', "element %d, beyond maxsize, was changed", i);
		wmemset(buf, L'#', 32);
		r = wcsftime(buf, 0, L"%F", &tm);
		pc_check(r == 0, "wcsftime(maxsize 0) returned %lu, expected 0", (unsigned long)r);
		for (i = 0; i < 32; i++)
			pc_check(buf[i] == L'#', "element %d was changed with maxsize 0", i);
	}
	pc_pass();
}
