/*
 * strcmp() of transformed strings agrees with strcoll() of the originals.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.strxfrm.002
 */
#include "lc.h"

#define DN 256
#define SIGN(v) (((v) > 0) - ((v) < 0))

static const char *const str[12] = { "", "a", "b", "ab", "A", "Z", "9", " ", "abc", "az", "Ab", "aa" };

int main(void)
{
	static char x[12][DN];
	size_t r;
	int i, j, c, k;

	pc_begin("XSH.strxfrm.002");
	lc_init();
	if (setlocale(LC_ALL, "C") == NULL)
		pc_unresolved("setlocale(LC_ALL, \"C\") failed");
	pc_test();
	for (i = 0; i < 12; i++) {
		r = strxfrm(x[i], str[i], DN);
		if (r >= DN)
			pc_unresolved("string %d needs %zu bytes, more than the %d provided", i, r, DN);
	}
	for (i = 0; i < 12; i++)
		for (j = 0; j < 12; j++) {
			k = strcmp(x[i], x[j]);
			c = strcoll(str[i], str[j]);
			pc_check(SIGN(k) == SIGN(c), "strings \"%s\" and \"%s\": strcmp() of the results gave %d, strcoll() gave %d", str[i], str[j], k, c);
		}
	pc_pass();
}
