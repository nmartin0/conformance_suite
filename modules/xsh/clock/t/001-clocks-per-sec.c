/*
 * CLOCKS_PER_SEC is one million.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.clock.001
 */
#include "pc.h"
#include <errno.h>
#include <time.h>
#include <unistd.h>

int main(void)
{
	pc_begin("XSH.clock.001");
	pc_test();
	pc_check(CLOCKS_PER_SEC == 1000000, "CLOCKS_PER_SEC is %ld", (long)CLOCKS_PER_SEC);
	pc_pass();
}
