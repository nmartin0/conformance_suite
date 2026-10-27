/*
 * Standard stream descriptors.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.stdin.001
 */
#include "pc.h"
#include <stdio.h>
#include <unistd.h>

int main(void)
{
	pc_begin("XSH.stdin.001");
	pc_test();
	pc_check(STDIN_FILENO == 0, "STDIN_FILENO is %d", STDIN_FILENO);
	pc_check(STDOUT_FILENO == 1, "STDOUT_FILENO is %d", STDOUT_FILENO);
	pc_check(STDERR_FILENO == 2, "STDERR_FILENO is %d", STDERR_FILENO);
	pc_check(fileno(stdin) == 0, "fileno(stdin) is %d", fileno(stdin));
	pc_check(fileno(stdout) == 1, "fileno(stdout) is %d", fileno(stdout));
	pc_check(fileno(stderr) == 2, "fileno(stderr) is %d", fileno(stderr));
	pc_pass();
}
