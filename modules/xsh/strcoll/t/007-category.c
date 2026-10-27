/*
 * strcoll() follows LC_COLLATE only.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.strcoll.007
 */
#include "coll.h"

int main(void)
{
	int i;

	pc_begin("XSH.strcoll.007");
	lc_init();
	lc_need();
	pc_test();
	if (setlocale(LC_ALL, "C") == NULL)
		pc_unresolved("setlocale(LC_ALL, \"C\") failed");
	for (i = 0; i < 6; i++)
		if (lc_cat[i] != LC_COLLATE && setlocale(lc_cat[i], lc_loc) == NULL)
			pc_unresolved("could not set category %d to the second locale", lc_cat[i]);
	coll_obj = (locale_t)0;
	coll_check_singles("LC_COLLATE left at POSIX");
	coll_check_pairs("LC_COLLATE left at POSIX");
	(void)setlocale(LC_ALL, "C");
	pc_pass();
}
