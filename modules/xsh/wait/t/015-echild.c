/*
 * wait() without children gives ECHILD.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.wait.015
 * MUTANT    wait.echild_as_einval
 */
#include "pc.h"
#include <errno.h>
#include <signal.h>
#include <string.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>

int main(void)
{
	int st;

	pc_begin("XSH.wait.015");
	pc_test();
	pc_expect_err(wait(&st), ECHILD, "wait() with no children");
	pc_pass();
}
