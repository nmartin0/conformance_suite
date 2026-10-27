/*
 * iswgraph: members.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.iswgraph.001
 */
#include "wc.h"
#include <string.h>

int main(void)
{
	pc_begin("XSH.iswgraph.001");
	wc_init();
	pc_test();
	wc_chk("iswgraph", iswgraph, C_GRAPH, 1, 1);
	pc_pass();
}
