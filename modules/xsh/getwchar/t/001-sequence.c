/*
 * getwchar() reads wide characters from standard input.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.getwchar.001
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

/* Opens path with flags and makes it descriptor target (setup). */
static void fd_onto(const char *path, int flags, int target)
{
	int fd = open(path, flags, 0666);

	pc_check(fd >= 0, "open(%s) failed: %s", path, pc_errname(errno));
	if (fd != target) {
		if (!(dup2(fd, target) == target))
			pc_unresolved("dup2 to %d failed: %s", target, pc_errname(errno));
		pc_check(close(fd) == 0, "close failed: %s", pc_errname(errno));
	}
}

int main(void)
{
	pc_begin("XSH.getwchar.001");
	{
		wint_t c;
		long pos;

		pc_make_file("f", "ab");
		fd_onto("f", O_RDONLY, 0);
		pc_test();
		c = getwchar();
		pc_check(c == L'a', "the first getwchar() returned %#lx, expected L'a'", (unsigned long)c);
		pos = ftell(stdin);
		NEED(pos != -1, "ftell() failed: %s", pc_errname(errno));
		pc_check(pos == 1, "ftell(stdin) after one call is %ld, expected 1", pos);
		c = getwchar();
		pc_check(c == L'b', "the second getwchar() returned %#lx, expected L'b'", (unsigned long)c);
		pos = ftell(stdin);
		NEED(pos != -1, "ftell() failed: %s", pc_errname(errno));
		pc_check(pos == 2, "ftell(stdin) after two calls is %ld, expected 2", pos);
		pc_check(fwide(stdin, 0) > 0, "stdin is not wide-oriented after getwchar()");
	}
	pc_pass();
}
