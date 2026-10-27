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
	pc_check(towlower(WEOF) == WEOF, "towlower(WEOF) did not return WEOF");
	pc_pass();
}
