/*
 * iswxdigit: weof.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.iswxdigit.003
 */
#include "wc.h"
#include <string.h>

int main(void)
{
	pc_begin("XSH.iswxdigit.003");
	pc_test();
	pc_check(iswxdigit(WEOF) == 0, "iswxdigit(WEOF) returned non-zero");
	pc_pass();
}
