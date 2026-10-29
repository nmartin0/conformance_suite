/*
 * localtime() under TZ=<zone> agrees with the zone file.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION CONV.tzif.005
 * MUTANT localtime.shifted
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

static const char *const zones[] = {
	"UTC", "Etc/GMT+5", "Etc/GMT-14", "America/New_York", "Europe/London",
	"Asia/Kolkata", "Australia/Lord_Howe", "America/Sao_Paulo", "Europe/Paris", NULL
};

/* Simple footer "name[+-]hh[:mm]" with no DST part: its offset and name. */
static int simple_footer(const struct tzf *f, long *utoff, char *abbr)
{
	char s[128];
	const char *q = s;
	size_t k = 0;
	long h, m = 0;
	int sign = 1;
	char *e;

	if (f->footlen == 0 || f->footlen >= sizeof s)
		return 0;
	memcpy(s, f->foot, f->footlen);
	s[f->footlen] = '\0';
	if (*q == '<') {
		for (q++; *q != '>' && *q != '\0' && k < 15; q++)
			abbr[k++] = *q;
		if (*q != '>')
			return 0;
		q++;
	} else {
		for (; isalpha((unsigned char)*q) && k < 15; q++)
			abbr[k++] = *q;
	}
	abbr[k] = '\0';
	if (*q == '-' || *q == '+')
		sign = *q++ == '-' ? -1 : 1;
	if (!isdigit((unsigned char)*q))
		return 0;
	h = strtol(q, &e, 10);
	if (*e == ':')
		m = strtol(e + 1, &e, 10);
	if (*e != '\0')
		return 0;		/* a DST part or seconds: not simple */
	*utoff = -(sign * (h * 3600 + m * 60));
	return 1;
}

static void agree(const char *zone, const char *abbr_expect, long utoff_expect, int dst_expect, time_t t)
{
	struct tm *tm;
	char abbr[16];

	tm = localtime(&t);
	pc_check(tm != NULL, "TZ=%s: localtime(%lld) fails", zone, (long long)t);
	(void)strftime(abbr, sizeof abbr, "%Z", tm);
	pc_check(tz_local_offset(tm, t) == utoff_expect, "TZ=%s at %lld: offset %lld, the zone file says %ld", zone, (long long)t, tz_local_offset(tm, t), utoff_expect);
	pc_check(strcmp(abbr, abbr_expect) == 0, "TZ=%s at %lld: abbreviation '%s', the zone file says '%s'", zone, (long long)t, abbr, abbr_expect);
	pc_check((tm->tm_isdst > 0) == (dst_expect != 0), "TZ=%s at %lld: tm_isdst %d, the zone file says %d", zone, (long long)t, tm->tm_isdst, dst_expect);
}

int main(void)
{
	const char *dir;
	int z, found = 0, bad;
	char path[1024];
	struct tzf f;
	size_t len;
	unsigned i, step, ty, pty;
	long uo;
	char ab[16];
	int skip;

	pc_begin("CONV.tzif.005");
	dir = tz_find_dir();
	if (dir == NULL)
		pc_notinuse("no time zone database directory found (TZDIR unset, none of the common locations has a UTC file)");
	if (sizeof(time_t) < 8)
		pc_notinuse("time_t is 32 bits: version 2+ instants cannot be set");
	(void)setenv("TZDIR", dir, 1);	/* a system that ignores it uses its own default */
	pc_test();
	for (z = 0; zones[z] != NULL; z++) {
		(void)snprintf(path, sizeof path, "%s/%s", dir, zones[z]);
		memset(&f, 0, sizeof f);
		f.buf = (unsigned char *)cv_slurp(path, &len);
		if (f.buf == NULL)
			continue;
		f.len = len;
		bad = tz_layout(&f, &skip) != NULL || f.ver == 0;
		if (bad) {
			free(f.buf);
			continue;
		}
		found++;
		(void)setenv("TZ", zones[z], 1);
		tzset();
		if (f.b2.h.tim == 0) {
			/* no transitions: the footer decides; compare when it is a plain offset */
			if (simple_footer(&f, &uo, ab)) {
				agree(zones[z], ab, uo, 0, 0);
				agree(zones[z], ab, uo, 0, 1000000000);
			}
			free(f.buf);
			continue;
		}
		step = f.b2.h.tim / 6 + 1;
		for (i = 0; i < f.b2.h.tim; i += step) {
			int64_t tr = tzb_trans(&f.b2, i);

			ty = tzb_ttype(&f.b2, i);
			agree(zones[z], tzb_abbr(&f.b2, ty), tzb_utoff(&f.b2, ty), tzb_isdst(&f.b2, ty), (time_t)tr);
			pty = i > 0 ? tzb_ttype(&f.b2, i - 1) : 0;
			agree(zones[z], tzb_abbr(&f.b2, pty), tzb_utoff(&f.b2, pty), tzb_isdst(&f.b2, pty), (time_t)(tr - 1));
		}
		free(f.buf);
	}
	if (found == 0)
		pc_notinuse("none of the sample zones is a version 2+ TZif file in %s", dir);
	pc_note("compared %d sample zones from %s", found, dir);
	pc_pass();
}
