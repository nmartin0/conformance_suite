/*
 * Range expressions in the POSIX locale.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.9.3-bre.008
 */
#include "rx.h"

static void run(int cf)
{
	rx_set("[a-a]", cf, "a", "b`");
	rx_set("[a-cx-z]", cf, "abcxyz", "dw`{");
	rx_set("[^a-cx-z]", cf, "dw`{", "abcxyz");
	rx_set("[b-d-]", cf, "bcd-", "ae+.");
	rx_set("[[.a.]-[.c.]]", cf, "abc", "d`");
	rx_set("[a-[.c.]]", cf, "abc", "d`");
	rx_set("[[.a.]-c]", cf, "abc", "d`");
	rx_set("[+-[.-.]]", cf, "+,-", "*.");
	rx_set("[[.-.]-0]", cf, "-./0", ",1");
	rx_set("[--/]", cf, "-./", ",0");
	rx_set("[ -~]", cf, " a~!", "\001\177\n\t");
	rx_set("[\001-\037]", cf, "\001\037\n\t", " a");
}

int main(void)
{
	pc_begin("XBD.9.3-bre.008");
	pc_test();
	run(0);
	run(REG_EXTENDED);
	pc_pass();
}
