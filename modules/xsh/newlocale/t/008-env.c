/*
 * newlocale() with "" reads the environment.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.newlocale.008
 */
#include "lc.h"

int main(void)
{
	static const char *const v[6] = { "LC_COLLATE", "LC_CTYPE", "LC_MESSAGES", "LC_MONETARY", "LC_NUMERIC", "LC_TIME" };
	locale_t obj;
	int i;

	pc_begin("XSH.newlocale.008");
	lc_init();
	lc_nosuch();
	pc_test();
	lc_env_clear();
	for (i = 0; i < 6; i++)
		lc_env_set(v[i], LC_NOSUCH);
	lc_env_set("LANG", LC_NOSUCH);
	lc_env_set("LC_ALL", "POSIX");
	obj = newlocale(LC_ALL_MASK, "", (locale_t)0);
	pc_check(obj != (locale_t)0, "newlocale(LC_ALL_MASK, \"\", 0) failed with LC_ALL=POSIX, errno %s", pc_errname(errno));
	lc_check_items(obj, "LC_ALL=POSIX");
	freelocale(obj);

	lc_env_clear();
	lc_env_set("LANG", "POSIX");
	obj = newlocale(LC_ALL_MASK, "", (locale_t)0);
	pc_check(obj != (locale_t)0, "newlocale(LC_ALL_MASK, \"\", 0) failed with LANG=POSIX, errno %s", pc_errname(errno));
	lc_check_items(obj, "LANG=POSIX");
	freelocale(obj);
	pc_pass();
}
