/*
 * Fixture for ci/mutate.sh: pcfy_other() returns 8; the only declaration
 * is a build shim, and the "system" behind it returns 7.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.pcfy.002
 * VARIANT   pcfy.shimmed
 */
#include "pc.h"

int main(void)
{
	pc_begin("XSH.pcfy.002");
	pc_test();
	pc_check(pcfy_other() == 8, "pcfy_other() returned %d", pcfy_other());
	pc_pass();
}
