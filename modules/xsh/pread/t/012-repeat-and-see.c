/*
 * pread() is repeatable and sees data written through another descriptor.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.pread.012
 */
#include "pc.h"
#include <errno.h>
#include <fcntl.h>
#include <limits.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/resource.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>

int main(void)
{
	char a[3], b[3];
	int w, r;

	pc_begin("XSH.pread.012");
	pc_require_space();
	w = open("f", O_RDWR | O_CREAT, 0600);
	pc_check(w >= 0, "cannot create f");
	pc_check(write(w, "abcdef", 6) == 6, "write failed");
	r = open("f", O_RDONLY);
	pc_check(r >= 0, "cannot reopen f");
	pc_test();
	memset(a, 0, sizeof a);
	memset(b, 0, sizeof b);
	pc_check(pread(r, a, 2, 2) == 2 && pread(r, b, 2, 2) == 2, "pread() did not return 2");
	pc_check(memcmp(a, "cd", 2) == 0 && memcmp(b, "cd", 2) == 0, "the two preads returned %.2s and %.2s", a, b);
	pc_check(lseek(w, 2, SEEK_SET) == 2 && write(w, "XY", 2) == 2, "second write failed");
	pc_check(pread(r, a, 2, 2) == 2 && memcmp(a, "XY", 2) == 0, "pread() did not see the later write");
	pc_pass();
}
