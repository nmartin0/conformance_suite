/*
 * strxfrm_l() uses its object and not the global locale.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.strxfrm.007
 */
#include "lc.h"

#define DN 256
#define SIGN(v) (((v) > 0) - ((v) < 0))

static const char *const str[5] = { "B", "a", "Z", "ab", "A" };

int main(void)
{
	static char x[5][DN];
	locale_t obj;
	size_t r;
	int i, j, c, k;

	pc_begin("XSH.strxfrm.007");
	lc_init();
	lc_need();
	obj = newlocale(LC_ALL_MASK, "C", (locale_t)0);
	if (obj == (locale_t)0)
		pc_unresolved("newlocale() could not make a locale object for \"C\"");
	pc_test();
	if (setlocale(LC_ALL, lc_loc) == NULL)
		pc_unresolved("could not select the second locale");
	for (i = 0; i < 5; i++) {
		r = strxfrm_l(x[i], str[i], DN, obj);
		if (r >= DN)
			pc_unresolved("string %d needs %zu bytes, more than the %d provided", i, r, DN);
	}
	pc_check(strcmp(x[0], x[1]) < 0, "the result for \"B\" did not compare below the result for \"a\"");
	for (i = 0; i < 5; i++)
		for (j = 0; j < 5; j++) {
			k = strcmp(x[i], x[j]);
			c = strcoll_l(str[i], str[j], obj);
			pc_check(SIGN(k) == SIGN(c), "strings \"%s\" and \"%s\": strcmp() of the results gave %d, strcoll_l() gave %d", str[i], str[j], k, c);
		}
	(void)setlocale(LC_ALL, "C");
	freelocale(obj);
	pc_pass();
}
