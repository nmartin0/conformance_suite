/*
 * gethostid() returns a stable 32-bit identifier.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.gethostid.001
 */
#include "pc.h"
#include <errno.h>
#include <signal.h>
#include <stdint.h>
#include <sys/wait.h>
#include <unistd.h>

int main(void)
{
	long a, b;
	pid_t c;
	int st;

	pc_begin("XSH.gethostid.001");
	pc_test();
	a = gethostid();
	b = gethostid();
	pc_check((a >= INT32_MIN && a <= INT32_MAX) || (a >= 0 && (unsigned long)a <= 0xFFFFFFFFUL),
	    "gethostid() returned %ld, which is not a 32-bit value", a);
	pc_check(a == b, "two calls returned %ld and %ld", a, b);
	c = fork();
	if (c == -1)
		pc_unresolved("fork failed: %s", pc_errname(errno));
	if (c == 0) {
		alarm(20);
		_exit(gethostid() == a ? 0 : 1);
	}
	while (waitpid(c, &st, 0) == -1)
		;
	pc_check(WIFEXITED(st) && WEXITSTATUS(st) == 0, "a child saw a different identifier (status %#x)", st);
	pc_pass();
}
