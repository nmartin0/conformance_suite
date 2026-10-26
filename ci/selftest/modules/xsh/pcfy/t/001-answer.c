/*
 * Fixture for ci/mutate.sh: pcfy_answer() returns 42.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.pcfy.001
 * MUTANT    pcfy.wrong
 * VARIANT   pcfy.provides
 */
#include "pc.h"

int main(void)
{
	pc_begin("XSH.pcfy.001");
	pc_test();
	pc_check(pcfy_answer() == 42, "pcfy_answer() returned %d", pcfy_answer());
	pc_pass();
}
