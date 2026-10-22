/* Harness self-test. Copyright (c) 2026 Nicholas Martin. All rights reserved. */
#include "pc.h"
#include <sys/wait.h>
#include <unistd.h>
int main(void)
{
	int st;
	pid_t p;
	pc_begin("ST.c.012");
	pc_test();
	p = fork();
	pc_check(p >= 0, "fork");
	if (p == 0)
		pc_fail("child failure must not become the result");
	pc_check(waitpid(p, &st, 0) == p, "waitpid");
	pc_check(WIFEXITED(st) && WEXITSTATUS(st) == 1, "child exit status %d", st);
	pc_pass();
}
