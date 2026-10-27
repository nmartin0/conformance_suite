/*
 * iswupper: upper lower mapping.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.iswupper.004
 */
#include "wc.h"
#include <string.h>

int main(void)
{
	int i;
	wint_t w;
	pc_begin("XSH.iswupper.004");
	wc_init();
	pc_test();
	for (i = 0; i < 128; i++) {
		if (!wc_in(C_UPPER, i))
			continue;
		w = wc_ch[i];
		pc_check(WC_NZ(iswupper), "relation failed at position %d: WC_NZ(iswupper)", i);
		pc_check(!WC_NZ(iswlower), "relation failed at position %d: !WC_NZ(iswlower)", i);
		pc_check(iswlower(towlower(w)) != 0, "relation failed at position %d: iswlower(towlower(w)) != 0", i);
		pc_check(towupper(towlower(w)) == w, "relation failed at position %d: towupper(towlower(w)) == w", i);
	}
	pc_pass();
}
