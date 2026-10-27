/*
 * strfmon_l() with an object for "C" is strfmon() in the POSIX locale.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.strfmon.013
 */
#include "fmon.h"

static void run(locale_t o, const char *w)
{
	char buf[64], dg[64];
	const char *m;
	ssize_t r;
	int e;

	fm_obj = o;
	r = strfmon_l(buf, sizeof buf, o, "a%%b");
	pc_check(r == 3 && strcmp(buf, "a%b") == 0, "%s: strfmon_l(\"a%%%%b\") returned %ld with \"%s\"", w, (long)r, buf);
	errno = 0;
	r = strfmon_l(buf, 3, o, "abc");
	e = errno;
	pc_check(r == -1 && e == E2BIG, "%s: strfmon_l(\"abc\", maxsize 3) returned %ld, errno %s, expected -1 and E2BIG", w, (long)r, pc_errname(e));
	r = strfmon_l(buf, 4, o, "abc");
	pc_check(r == 3 && strcmp(buf, "abc") == 0, "%s: strfmon_l(\"abc\", maxsize 4) returned %ld with \"%s\"", w, (long)r, buf);
	fm_want("%.2n", 123.456, "12346");
	r = strfmon_l(buf, sizeof buf, o, "%.0n", -123.0);
	pc_check(r > 0, "%s: strfmon_l(\"%%.0n\", -123.0) failed, errno %s", w, pc_errname(errno));
	m = strchr(buf, '-');
	fm_digits(buf, dg, sizeof dg);
	pc_check(m != NULL && strcmp(dg, "123") == 0 && m < strchr(buf, '1'), "%s: strfmon_l(\"%%.0n\", -123.0) gave \"%s\"", w, buf);
	errno = 0;
	r = strfmon_l(buf, sizeof buf, o, "%+.0n", 1.0);
	e = errno;
	pc_check(r == -1 && e == EINVAL, "%s: strfmon_l(\"%%+.0n\") returned %ld, errno %s, expected -1 and EINVAL", w, (long)r, pc_errname(e));
}

int main(void)
{
	locale_t obj;
	int have;

	pc_begin("XSH.strfmon.013");
	lc_init();
	have = lc_find();
	obj = newlocale(LC_ALL_MASK, "C", (locale_t)0);
	if (obj == (locale_t)0 || setlocale(LC_ALL, "C") == NULL)
		pc_unresolved("could not set up the POSIX locale and object");
	pc_test();
	run(obj, "POSIX global locale");
	if (have) {
		if (setlocale(LC_ALL, lc_loc) == NULL)
			pc_unresolved("could not select the second locale");
		run(obj, "other global locale");
		(void)setlocale(LC_ALL, "C");
	}
	freelocale(obj);
	pc_pass();
}
