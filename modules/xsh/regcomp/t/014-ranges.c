/*
 * Hyphen placement and range expressions in the POSIX locale.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.regcomp.014
 */
#include "rx.h"

static void run(int cf)
{
	rx_set("[-ac]", cf, "-ac", "b");
	rx_set("[ac-]", cf, "-ac", "b");
	rx_set("[^-ac]", cf, "b", "-ac");
	rx_set("[^ac-]", cf, "b", "-ac");
	rx_set("[%--]", cf, "%&'()*+,-", ".$/");
	rx_set("[--@]", cf, "-./0@", ",A");
	rx_set("[a-c]", cf, "abc", "d`");
	rx_set("[^a-c]", cf, "d`", "abc");
	rx_set("[][.-.]-0]", cf, "]-./0", "1,a");
	rx_set("[0-9]", cf, "09", "/:");
	rx_set("[A-Za-z]", cf, "AZaz", "@[`{");
}

int main(void)
{
	pc_begin("XSH.regcomp.014");
	pc_test();
	run(0);
	run(REG_EXTENDED);
	pc_pass();
}
