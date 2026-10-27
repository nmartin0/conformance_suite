/*
 * swprintf() fails with EOVERFLOW when n or more characters are requested.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.fwprintf.013
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
	pc_begin("XSH.fwprintf.013");
	{
		wchar_t buf[16];
		int r, e, i, nul = 0;

		wmemset(buf, L'#', 16);
		pc_test();
		errno = 0;
		r = swprintf(buf, 4, L"abcdef");
		e = errno;
		pc_check(r < 0, "swprintf() returned %d for output longer than n, expected a negative value", r);
		pc_check(e == EOVERFLOW, "swprintf() set errno %s, expected EOVERFLOW", pc_errname(e));
		for (i = 4; i < 16; i++)
			pc_check(buf[i] == L'#', "element %d, beyond n, was changed", i);
		for (i = 0; i < 4; i++)
			if (buf[i] == L'\0')
				nul = 1;
		pc_check(nul, "no null wide character among the first four elements");
	}
	pc_pass();
}
