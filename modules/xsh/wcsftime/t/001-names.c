/*
 * The POSIX locale's day, month and AM/PM names, as wide characters.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.wcsftime.001
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
	pc_begin("XSH.wcsftime.001");
	{
		static const char *const wd[7] = { "Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat" };
		static const char *const wdl[7] = { "Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday" };
		static const char *const mo[12] = { "Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec" };
		static const char *const mol[12] = { "January", "February", "March", "April", "May", "June", "July", "August", "September", "October", "November", "December" };
		static const int hrs[4] = { 0, 11, 12, 23 };
		static const char *const ap[4] = { "AM", "AM", "PM", "PM" };
		struct tm tm;
		int i;

		pc_test();
		for (i = 0; i < 7; i++) {
			when(&tm, 100, 0, 1, i, 0, 0, 0, 0);
			chk(&tm, L"%a", wd[i]);
			chk(&tm, L"%A", wdl[i]);
		}
		for (i = 0; i < 12; i++) {
			when(&tm, 100, i, 1, 0, 0, 0, 0, 0);
			chk(&tm, L"%b", mo[i]);
			chk(&tm, L"%h", mo[i]);
			chk(&tm, L"%B", mol[i]);
		}
		for (i = 0; i < 4; i++) {
			when(&tm, 100, 0, 1, 0, 0, hrs[i], 0, 0);
			chk(&tm, L"%p", ap[i]);
		}
	}
	pc_pass();
}
