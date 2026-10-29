/*
 * TZif files: the footer is a POSIX TZ string consistent with the last transition.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION CONV.tzif.003
 */
#include <sys/stat.h>
#include <ctype.h>
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "pc.h"
#include "../../../../lib/cv.h"
#include "../../../../lib/tzif.h"

/* A POSIX-style name: three or more letters, or <...> of alphanumerics, plus and minus. */
static const char *name(const char *s, const char **end)
{
	const char *q = s;

	if (*q == '<') {
		q++;
		while (*q != '>' && *q != '\0' && (isalnum((unsigned char)*q) || *q == '+' || *q == '-'))
			q++;
		if (*q != '>' || q - s - 1 < 3)
			return "bad quoted time zone name";
		*end = q + 1;
		return NULL;
	}
	while (isalpha((unsigned char)*q))
		q++;
	if (q - s < 3)
		return "time zone name shorter than three letters";
	*end = q;
	return NULL;
}

/* [+-]hh[:mm[:ss]]; maxh the largest hour; signed allowed when sgn */
static const char *hms(const char **sp, int sgn, int maxh)
{
	const char *q = *sp;
	long h;
	char *e;

	if (*q == '+' || *q == '-') {
		if (!sgn)
			return "unexpected sign";
		q++;
	}
	if (!isdigit((unsigned char)*q))
		return "hour expected";
	h = strtol(q, &e, 10);
	if (h > maxh || e - q > 3)
		return "hour out of range";
	q = e;
	if (*q == ':') {
		long v = strtol(q + 1, &e, 10);

		if (e == q + 1 || v > 59)
			return "minutes out of range";
		q = e;
		if (*q == ':') {
			v = strtol(q + 1, &e, 10);
			if (e == q + 1 || v > 59)
				return "seconds out of range";
			q = e;
		}
	}
	*sp = q;
	return NULL;
}

static const char *rule(const char **sp, int v3)
{
	const char *q = *sp, *e;
	char *x;
	long m, w, d;

	if (*q == 'M') {
		m = strtol(q + 1, &x, 10);
		if (*x != '.' || m < 1 || m > 12)
			return "bad month in M rule";
		w = strtol(x + 1, &x, 10);
		if (*x != '.' || w < 1 || w > 5)
			return "bad week in M rule";
		d = strtol(x + 1, &x, 10);
		if (d < 0 || d > 6)
			return "bad weekday in M rule";
		q = x;
	} else {
		if (*q == 'J')
			q++;
		if (!isdigit((unsigned char)*q))
			return "rule date expected";
		d = strtol(q, &x, 10);
		if (d > 365)
			return "day of year out of range";
		q = x;
	}
	if (*q == '/') {
		q++;
		e = hms(&q, v3, v3 ? 167 : 24);
		if (e != NULL)
			return e;
	}
	*sp = q;
	return NULL;
}

/* The syntax of RFC 8536 3.3 (POSIX TZ; version 3 may use the extensions). */
static const char *tz_syntax(const char *s, int v3)
{
	const char *q = s, *e;

	if (*s == '\0')
		return NULL;
	if ((e = name(q, &q)) != NULL)
		return e;
	if ((e = hms(&q, 1, 24)) != NULL)
		return e;
	if (*q == '\0')
		return NULL;
	if ((e = name(q, &q)) != NULL)
		return e;
	if (*q != ',' && *q != '\0' && (e = hms(&q, 1, 24)) != NULL)
		return e;
	if (*q == '\0')
		return NULL;
	if (*q++ != ',' || (e = rule(&q, v3)) != NULL)
		return e != NULL ? e : "comma expected";
	if (*q++ != ',' || (e = rule(&q, v3)) != NULL)
		return e != NULL ? e : "second rule expected";
	return *q == '\0' ? NULL : "text after the second rule";
}

/* Does a version 3 string use what only version 3 allows? */
static int uses_extension(const char *s)
{
	const char *q;

	for (q = s; (q = strchr(q, '/')) != NULL; q++)
		if (q[1] == '-' || q[1] == '+' || (isdigit((unsigned char)q[1]) && strtol(q + 1, NULL, 10) > 24))
			return 1;
	return 0;
}

static void each(const char *path, const char *rel)
{
	struct tzf f;
	size_t len;
	char foot[512], abbr[16];
	int skip, v3;
	const char *e;
	unsigned last, ty;
	time_t t;
	struct tm *tm;

	memset(&f, 0, sizeof f);
	f.buf = (unsigned char *)cv_slurp(path, &len);
	pc_check(f.buf != NULL, "%s cannot be read", rel);
	f.len = len;
	if (tz_layout(&f, &skip) != NULL || f.ver == 0) {
		free(f.buf);
		return;
	}
	pc_check(f.footlen < sizeof foot, "%s: footer is %zu bytes long", rel, f.footlen);
	memcpy(foot, f.foot, f.footlen);
	foot[f.footlen] = '\0';
	v3 = f.ver == '3';
	e = tz_syntax(foot, v3);
	pc_check(e == NULL, "%s: footer '%s' is not a %s TZ string: %s", rel, foot, v3 ? "POSIX (or version 3)" : "POSIX", e);
	if (foot[0] == '\0' || f.b2.h.tim == 0 || sizeof(time_t) < 8 || foot[0] == ':' || (v3 && uses_extension(foot))) {
		free(f.buf);
		return;
	}
	/* evaluate the footer at the last transition: it must give that transition's type */
	last = f.b2.h.tim - 1;
	ty = tzb_ttype(&f.b2, last);
	t = (time_t)tzb_trans(&f.b2, last);
	(void)setenv("TZ", foot, 1);
	tzset();
	tm = localtime(&t);
	pc_check(tm != NULL, "%s: localtime() fails for footer '%s'", rel, foot);
	(void)strftime(abbr, sizeof abbr, "%Z", tm);
	pc_check(tz_local_offset(tm, t) == tzb_utoff(&f.b2, ty) && (tm->tm_isdst > 0) == (tzb_isdst(&f.b2, ty) != 0) &&
	    strcmp(abbr, tzb_abbr(&f.b2, ty)) == 0,
	    "%s: footer '%s' at the last transition gives offset %lld, dst %d, '%s'; the file says %ld, dst %d, '%s'",
	    rel, foot, tz_local_offset(tm, t), tm->tm_isdst > 0, abbr, (long)tzb_utoff(&f.b2, ty),
	    tzb_isdst(&f.b2, ty) != 0, tzb_abbr(&f.b2, ty));
	(void)unsetenv("TZ");
	free(f.buf);
}

int main(void)
{
	const char *dir;
	int count = 0;

	pc_begin("CONV.tzif.003");
	dir = tz_find_dir();
	if (dir == NULL)
		pc_notinuse("no time zone database directory found (TZDIR unset, none of the common locations has a UTC file)");
	pc_test();
	(void)tz_walk(dir, "", 0, 20000, &count, each);
	pc_check(count > 0, "no TZif file below %s", dir);
	pc_note("checked %d TZif files below %s", count, dir);
	pc_pass();
}
