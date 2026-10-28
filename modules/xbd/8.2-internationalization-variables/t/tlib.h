/*
 * tlib.h - helpers that write LC_TIME locale source text for the tests of
 * this module. Strings are written with the symbolic names of the
 * portable characters, so nothing depends on the encoding.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 */
#ifndef TLIB_H
#define TLIB_H

#include "ldef.h"
#include "pcchars.h"
#include <time.h>

static const char *const tl_abday[7] = { "ua", "ub", "uc", "ud", "ue", "uf", "ug" };
static const char *const tl_day[7] = { "uaday", "ubday", "ucday", "udday", "ueday", "ufday", "ugday" };
static const char *const tl_abmon[12] = { "ma", "mb", "mc", "md", "me", "mf", "mg", "mh", "mi", "mj", "mk", "ml" };
static const char *const tl_mon[12] = { "mona", "monb", "monc", "mond", "mone", "monf", "mong", "monh", "moni", "monj",
    "monk", "monl" };
static const char *const tl_abalt[12] = { "na", "nb", "nc", "nd", "ne", "nf", "ng", "nh", "ni", "nj", "nk", "nl" };
static const char *const tl_alt[12] = { "nona", "nonb", "nonc", "nond", "none", "nonf", "nong", "nonh", "noni", "nonj",
    "nonk", "nonl" };

/* Append a quoted string in symbolic form; a tab character is written as the escape sequence. */
static size_t tl_str(char *out, size_t n, size_t len, const char *s)
{
	char nb[4];
	int w;

	w = snprintf(out + len, n - len, "\"");
	len += (size_t)w;
	for (; *s != '\0' && len + 40 < n; s++) {
		if (*s == '\t')
			w = snprintf(out + len, n - len, "\\t");
		else
			w = snprintf(out + len, n - len, "<%s>", pcc_sym(*s, nb));
		len += (size_t)w;
	}
	w = snprintf(out + len, n - len, "\"");
	return len + (size_t)w;
}

/* Append "keyword v0;v1;...\n" with every value a quoted string. */
static size_t tl_list(char *out, size_t n, size_t len, const char *kw, const char *const *v, int cnt)
{
	int i;

	len += (size_t)snprintf(out + len, n - len, "%s ", kw);
	for (i = 0; i < cnt; i++) {
		if (i > 0)
			len += (size_t)snprintf(out + len, n - len, ";");
		len = tl_str(out, n, len, v[i]);
	}
	len += (size_t)snprintf(out + len, n - len, "\n");
	return len;
}

/* Append "keyword value\n" with one quoted string. */
static size_t tl_one(char *out, size_t n, size_t len, const char *kw, const char *val)
{
	len += (size_t)snprintf(out + len, n - len, "%s ", kw);
	len = tl_str(out, n, len, val);
	len += (size_t)snprintf(out + len, n - len, "\n");
	return len;
}

/* Start an LC_TIME category with the name lists; the caller continues at the returned length. */
static size_t tl_start(char *out, size_t n)
{
	size_t len = (size_t)snprintf(out, n, "LC_TIME\n");

	len = tl_list(out, n, len, "abday", tl_abday, 7);
	len = tl_list(out, n, len, "day", tl_day, 7);
	len = tl_list(out, n, len, "abmon", tl_abmon, 12);
	len = tl_list(out, n, len, "ab_alt_mon", tl_abalt, 12);
	len = tl_list(out, n, len, "mon", tl_mon, 12);
	len = tl_list(out, n, len, "alt_mon", tl_alt, 12);
	return len;
}

/* The default formats, then the end of the category. */
static size_t tl_formats(char *out, size_t n, size_t len)
{
	const char *ampm[2] = { "ante", "post" };

	len = tl_one(out, n, len, "d_t_fmt", "%d.%m.%y %H:%M:%S");
	len = tl_one(out, n, len, "d_fmt", "%d.%m.%y");
	len = tl_one(out, n, len, "t_fmt", "%H:%M:%S");
	len = tl_list(out, n, len, "am_pm", ampm, 2);
	len = tl_one(out, n, len, "t_fmt_ampm", "%I:%M:%S %p");
	return len;
}

static size_t tl_end(char *out, size_t n, size_t len)
{
	return len + (size_t)snprintf(out + len, n - len, "END LC_TIME\n");
}

/* A time with the given weekday and month, and fixed other fields (2000-03-15 14:05:09 for the defaults). */
static struct tm tl_tm(int wday, int mon)
{
	struct tm t;

	memset(&t, 0, sizeof t);
	t.tm_year = 100;
	t.tm_mon = mon;
	t.tm_mday = 15;
	t.tm_hour = 14;
	t.tm_min = 5;
	t.tm_sec = 9;
	t.tm_wday = wday;
	t.tm_yday = 74;
	return t;
}

#endif
