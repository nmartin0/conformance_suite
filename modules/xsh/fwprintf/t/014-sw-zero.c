/*
 * swprintf() with n 0 writes nothing and fails.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.fwprintf.014
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
	pc_begin("XSH.fwprintf.014");
	{
		wchar_t buf[8];
		int r, i, e;

		wmemset(buf, L'#', 8);
		pc_test();
		errno = 0;
		r = swprintf(buf, 0, L"abc");
		e = errno;
		pc_check(r < 0, "swprintf() with n 0 returned %d, expected a negative value", r);
		pc_check(e == EOVERFLOW, "swprintf() with n 0: errno is %s, expected EOVERFLOW", pc_errname(e));
		for (i = 0; i < 8; i++)
			pc_check(buf[i] == L'#', "element %d was changed with n 0", i);
	}
	pc_pass();
}
