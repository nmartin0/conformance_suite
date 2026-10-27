/*
 * iswctype: utf8 portable.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.iswctype.005
 */
#include "wc.h"
#include <string.h>

static const struct { int cls, inside, want; } ut[] = {
	{ C_ALNUM, 1, 1 }, { C_ALPHA, 1, 1 }, { C_BLANK, 1, 1 }, { C_DIGIT, 1, 1 }, { C_DIGIT, 0, 0 },
	{ C_GRAPH, -2, 1 }, { C_LOWER, 1, 1 }, { C_PRINT, -2, 1 }, { C_SPACE, 1, 1 }, { C_UPPER, 1, 1 },
	{ C_XDIGIT, 1, 1 }, { C_XDIGIT, 0, 0 }
};

int main(void)
{
	int j, k, i;
	wctype_t h[C_N];
	pc_begin("XSH.iswctype.005");
	wc_init();
	wc_utf8();
	for (k = 0; k < C_N; k++) {
		h[k] = wctype(wc_cname[k]);
		pc_check(h[k] != (wctype_t)0, "wctype(\"%s\") returned 0", wc_cname[k]);
	}
	pc_test();
	for (j = 0; j < (int)(sizeof ut / sizeof ut[0]); j++) {
		k = ut[j].cls;
		for (i = 0; i < 128; i++) {
			if (ut[j].inside == -2) {
				if (!wc_in(C_ALNUM, i) && !(k == C_PRINT && i == 32))
					continue;
			} else if (wc_in(k, i) != ut[j].inside)
				continue;
			pc_check((iswctype(wc_ch[i], h[k]) != 0) == ut[j].want, "iswctype() with the %s handle is wrong at position %d", wc_cname[k], i);
		}
	}
	pc_pass();
}
