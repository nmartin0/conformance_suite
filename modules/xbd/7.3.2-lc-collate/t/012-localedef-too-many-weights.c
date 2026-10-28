/*
 * An order_start with more operands than {COLL_WEIGHTS_MAX} allows makes
 * localedef issue a warning, which shows as exit status 1 when the locale
 * is made nonetheless (-c); with exactly the maximum number of operands
 * the status is not a warning for that reason.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.7.3.2-lc-collate.012
 */
#include "pc.h"
#include "ldef.h"

#define REST LD_CP_CTYPE LD_CP_MONETARY LD_CP_NUMERIC LD_CP_TIME LD_CP_MESSAGES

static size_t add(char *out, size_t n, size_t len, const char *first, const char *rest, long levels)
{
	long i;

	len += (size_t)snprintf(out + len, n - len, "%s", first);
	for (i = 1; i < levels; i++)
		len += (size_t)snprintf(out + len, n - len, "%s", rest);
	return len;
}

static void build(char *out, size_t n, long levels)
{
	size_t len = 0;

	len = add(out, n, len, "LC_COLLATE\norder_start forward", ";forward", levels);
	len = add(out, n, len, "\n<a> <a>", ";<a>", levels);
	len = add(out, n, len, "\n<b> <b>", ";<b>", levels);
	len = add(out, n, len, "\nUNDEFINED IGNORE", ";...", levels);
	(void)snprintf(out + len, n - len, "\norder_end\nEND LC_COLLATE\n");
}

int main(void)
{
	static char text[8000];
	char loc[4200];
	long max;
	int rc;

	pc_begin("XBD.7.3.2-lc-collate.012");
	ld_need();
	max = sysconf(_SC_COLL_WEIGHTS_MAX);
	if (max < 1)
		pc_notinuse("the system reports no {COLL_WEIGHTS_MAX} limit");
	if (max > 20)
		pc_notinuse("{COLL_WEIGHTS_MAX} is %ld, too large to build a source with one operand more", max);
	ld_charmap("cmap", LD_HDR, '\\', "", "", 0);
	build(text, sizeof text, max + 1);
	(void)snprintf(text + strlen(text), sizeof text - strlen(text), "%s", REST);
	pc_make_file("many.src", text);
	ld_path(loc, sizeof loc, "many.loc");
	rc = ld_run("-c", "cmap", "many.src", loc);
	if (rc > 1) {
		ld_errnote();
		pc_unresolved("localedef refused the test source (status %d)", rc);
	}
	pc_test();
	pc_check(rc == 1, "localedef exited with status %d for an order_start with %ld operands, one above {COLL_WEIGHTS_MAX}; the status for a warning is 1", rc, max + 1);
	pc_pass();
}
