/*
 * towupper: weof.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.towupper.003
 */
#include "wc.h"
#include <string.h>

int main(void)
{
	pc_begin("XSH.towupper.003");
	pc_test();
	/* control: a letter is converted, so WEOF coming back as WEOF is not the answer of a function that returns -1 */
	pc_check(towupper(L'a') == L'A', "towupper(L'a') did not return L'A'");
	pc_check(towupper(WEOF) == WEOF, "towupper(WEOF) did not return WEOF");
	pc_pass();
}
