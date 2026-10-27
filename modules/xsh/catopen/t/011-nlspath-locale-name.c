/*
 * [XSI] NLSPATH: with NL_CAT_LOCALE, %L is the name of the LC_MESSAGES locale.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.catopen.011
 */
#include "cg.h"
#include <locale.h>

int main(void)
{
	char loc[128], path[200];
	const char *ln;
	nl_catd cd;
	const char *m;

	pc_begin("XSH.catopen.011");
	cg_nlspath_guard();
	ln = setlocale(LC_MESSAGES, NULL);
	if (ln == NULL || strlen(ln) >= sizeof loc || strchr(ln, '/') != NULL || strchr(ln, ':') != NULL ||
	    strchr(ln, '%') != NULL || ln[0] == '\0')
		pc_unresolved("cannot use the locale name as a directory name");
	(void)snprintf(loc, sizeof loc, "%s", ln);
	pc_make_dir(loc);
	(void)snprintf(path, sizeof path, "%s/c011x", loc);
	cg_make(path, "c011.msg", CG_SRC);
	cg_setpath("./%L/%N");
	pc_test();
	errno = 0;
	cd = catopen("c011x", NL_CAT_LOCALE);
	pc_check(cd != CG_BAD, "catopen with %%L (locale \"%s\") failed: %s", loc, pc_errname(errno));
	m = catgets(cd, 1, 1, "default");
	pc_check(strcmp(m, "Hello catalog") == 0, "message is \"%s\"", m);
	pc_check(catclose(cd) == 0, "catclose failed");
	pc_pass();
}
