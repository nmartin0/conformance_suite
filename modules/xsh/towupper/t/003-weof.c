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
	pc_check(towupper(WEOF) == WEOF, "towupper(WEOF) did not return WEOF");
	pc_pass();
}
