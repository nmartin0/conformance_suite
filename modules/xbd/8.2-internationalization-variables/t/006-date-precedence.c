/*
 * The date utility takes its weekday name from LC_ALL, then LC_TIME, then
 * LANG, in that order: the name is that of locale X or of the POSIX locale
 * according to which variable comes first.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.8.2-internationalization-variables.006
 */
#include "pc.h"
#include "xloc.h"

static void run(const char *assigns, const char *fmt, char *out, size_t n)
{
	char cmd[9000];
	FILE *p;
	size_t l;

	(void)snprintf(cmd, sizeof cmd,
	    "unset LC_ALL LC_TIME LC_CTYPE LC_COLLATE LC_MESSAGES LC_MONETARY LC_NUMERIC LANG; %s date -u '%s' 2>/dev/null", assigns,
	    fmt);
	p = popen(cmd, "r");
	if (p == NULL)
		pc_unresolved("popen failed");
	if (fgets(out, (int)n, p) == NULL)
		out[0] = '\0';
	(void)pclose(p);
	l = strlen(out);
	if (l > 0 && out[l - 1] == '\n')
		out[l - 1] = '\0';
}

int main(void)
{
	char a1[100], a2[100], posix[100], gotv[5][100], assigns[9000];
	int w, k, ok = 0, i;
	static const char *const cases[5][2] = {
		{ "LC_ALL=POSIX LC_TIME='%s' LANG='%s'", "P" },
		{ "LC_TIME='%s' LANG=POSIX", "X" },
		{ "LANG='%s'", "X" },
		{ "LC_TIME=POSIX LANG='%s'", "P" },
		{ "LC_ALL='%s' LC_TIME=POSIX", "X" },
	};

	pc_begin("XBD.8.2-internationalization-variables.006");
	ld_need();
	xl_make();
	pc_test();
	for (k = 0; k < 3 && !ok; k++) {
		run("LC_ALL=POSIX", "+%w", a1, sizeof a1);
		run("LC_ALL=POSIX", "+%A", posix, sizeof posix);
		for (i = 0; i < 5; i++) {
			(void)snprintf(assigns, sizeof assigns, cases[i][0], xl_loc, xl_loc);
			run(assigns, "+%A", gotv[i], sizeof gotv[i]);
		}
		run("LC_ALL=POSIX", "+%w", a2, sizeof a2);
		if (strcmp(a1, a2) == 0)
			ok = 1;
	}
	if (!ok)
		pc_unresolved("the date changed during every attempt");
	if (sscanf(a1, "%d", &w) != 1 || w < 0 || w > 6)
		pc_unresolved("the date utility's numeric output \"%s\" is not understood", a1);
	for (i = 0; i < 5; i++) {
		(void)snprintf(assigns, sizeof assigns, cases[i][0], xl_loc, xl_loc);
		if (cases[i][1][0] == 'P')
			pc_check(strcmp(gotv[i], posix) == 0, "case %d (LC_ALL/LC_TIME/LANG order): the weekday is \"%s\", not the POSIX locale's \"%s\"", i + 1, gotv[i], posix);
		else
			pc_check(strcmp(gotv[i], tl_day[w]) == 0, "case %d (LC_ALL/LC_TIME/LANG order): the weekday is \"%s\", not locale X's \"%s\"", i + 1, gotv[i], tl_day[w]);
	}
	pc_pass();
}
