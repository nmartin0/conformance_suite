/*
 * iswgraph: weof.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.iswgraph.003
 */
#include "wc.h"
#include <string.h>

int main(void)
{
	pc_begin("XSH.iswgraph.003");
	pc_test();
	pc_check(iswgraph(WEOF) == 0, "iswgraph(WEOF) returned non-zero");
	pc_pass();
}
