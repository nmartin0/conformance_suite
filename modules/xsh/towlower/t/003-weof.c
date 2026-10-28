/*
 * towlower: weof.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.towlower.003
 */
#include "wc.h"
#include <string.h>

int main(void)
{
	pc_begin("XSH.towlower.003");
	pc_test();
	/* control: a letter is converted, so WEOF coming back as WEOF is not the answer of a function that returns -1 */
	pc_check(towlower(L'A') == L'a', "towlower(L'A') did not return L'a'");
	pc_check(towlower(WEOF) == WEOF, "towlower(WEOF) did not return WEOF");
	pc_pass();
}
