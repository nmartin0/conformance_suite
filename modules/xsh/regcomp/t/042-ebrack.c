/*
 * Unbalanced brackets and unknown character classes are rejected by regcomp.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.regcomp.042
 */
#include "rx.h"

int main(void)
{
	pc_begin("XSH.regcomp.042");
	pc_test();
	rx_bad("[a", 0);
	rx_bad("[a", REG_EXTENDED);
	rx_bad("[[:alpha:]", 0);
	rx_bad("[[:nosuchclass:]]", 0);
	rx_bad("[[:nosuchclass:]]", REG_EXTENDED);
	pc_pass();
}
