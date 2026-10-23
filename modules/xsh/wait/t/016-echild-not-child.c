/*
 * waitpid() of a non-child gives ECHILD.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.wait.016
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

	pc_begin("XSH.wait.016");
	pc_test();
	pc_expect_err(waitpid(getppid(), &st, 0), ECHILD, "waitpid(<the parent>)");
	pc_pass();
}
