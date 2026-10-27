/*
 * strxfrm_l() with an object for "C".
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.strxfrm.005
 */
#include "lc.h"

#define DN 256
#define SIGN(v) (((v) > 0) - ((v) < 0))

static const char *const str[6] = { "", "a", "b", "ab", "A", "9" };

int main(void)
{
	static char x[6][DN];
	locale_t obj;
	size_t r;
	int i, j, c, k;

	pc_begin("XSH.strxfrm.005");
	lc_init();
	obj = newlocale(LC_ALL_MASK, "C", (locale_t)0);
	if (obj == (locale_t)0)
		pc_unresolved("newlocale() could not make a locale object for \"C\"");
	pc_test();
	for (i = 0; i < 6; i++) {
		r = strxfrm_l(x[i], str[i], DN, obj);
		if (r >= DN)
			pc_unresolved("string %d needs %zu bytes, more than the %d provided", i, r, DN);
		pc_check(strlen(x[i]) == r, "strxfrm_l() of \"%s\" returned %zu but the stored string has length %zu", str[i], r, strlen(x[i]));
		pc_check(strxfrm_l(NULL, str[i], 0, obj) == r, "strxfrm_l(NULL, \"%s\", 0) did not return the full length %zu", str[i], r);
	}
	for (i = 0; i < 6; i++)
		for (j = 0; j < 6; j++) {
			k = strcmp(x[i], x[j]);
			c = strcoll_l(str[i], str[j], obj);
			pc_check(SIGN(k) == SIGN(c), "strings \"%s\" and \"%s\": strcmp() of the results gave %d, strcoll_l() gave %d", str[i], str[j], k, c);
		}
	freelocale(obj);
	pc_pass();
}
