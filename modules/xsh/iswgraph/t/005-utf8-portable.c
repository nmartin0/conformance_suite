/*
 * iswgraph: utf8 portable.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.iswgraph.005
 */
#include "wc.h"
#include <string.h>

int main(void)
{
	pc_begin("XSH.iswgraph.005");
	wc_init();
	wc_utf8();
	pc_test();
	wc_chk("iswgraph", iswgraph, C_ALNUM, 1, 1);
	wc_one("iswgraph", iswgraph, 32, 0);
	pc_pass();
}
