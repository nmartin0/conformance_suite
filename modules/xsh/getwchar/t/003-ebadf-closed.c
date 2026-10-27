/*
 * getwchar() with descriptor 0 closed fails with EBADF.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.getwchar.003
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
	pc_begin("XSH.getwchar.003");
	{
		wint_t r;
		int e;

		pc_make_file("f", "0123");
		fd_onto("f", O_RDONLY, 0);
		pc_test();
		NEED(close(0) == 0, "close(0) failed: %s", pc_errname(errno));
		errno = 0;
		r = getwchar();
		e = errno;
		pc_check(r == WEOF, "getwchar() on a closed descriptor returned %#lx, expected WEOF", (unsigned long)r);
		pc_check(e == EBADF, "getwchar() set errno %s, expected EBADF", pc_errname(e));
		pc_check(ferror(stdin) != 0, "the error indicator of stdin is not set");
	}
	pc_pass();
}
