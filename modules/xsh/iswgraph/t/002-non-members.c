/*
 * iswgraph: non members.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.iswgraph.002
 */
#include "wc.h"
#include <string.h>

int main(void)
{
	pc_begin("XSH.iswgraph.002");
	wc_init();
	pc_test();
	wc_chk("iswgraph", iswgraph, C_GRAPH, 0, 0);
	pc_pass();
}
