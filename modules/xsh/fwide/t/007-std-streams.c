/*
 * The predefined streams have no orientation at program start.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.fwide.007
 */
#include "pc.h"
#include <errno.h>
#include <fcntl.h>
#include <limits.h>
#include <locale.h>
#include <signal.h>
#include <stdarg.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>
#include <wchar.h>

/* After pc_test(): a step that is not under test gives UNRESOLVED, not FAIL. */
#define NEED(cond, ...) do { if (!(cond)) pc_unresolved(__VA_ARGS__); } while (0)

int main(void)
{
	pc_begin("XSH.fwide.007");
	{
		int a, b, c;

		pc_test();
		a = fwide(stdin, 0);
		b = fwide(stdout, 0);
		c = fwide(stderr, 0);
		pc_check(a == 0, "fwide(stdin, 0) at program start returned %d, expected 0", a);
		pc_check(b == 0, "fwide(stdout, 0) at program start returned %d, expected 0", b);
		pc_check(c == 0, "fwide(stderr, 0) at program start returned %d, expected 0", c);
	}
	pc_pass();
}
