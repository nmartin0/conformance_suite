/*
 * mkostemp() with O_APPEND opens the file in append mode.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.mkdtemp.017
 */
#include "pc.h"
#include <errno.h>
#include <fcntl.h>
#include <limits.h>
#include <poll.h>
#include <signal.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/resource.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>

/* After pc_test(): a step that is not under test gives UNRESOLVED, not FAIL. */
#define NEED(cond, ...) do { if (!(cond)) pc_unresolved(__VA_ARGS__); } while (0)


int main(void)
{
	char t[] = "fXXXXXX", buf[8];
	int fd, fl;

	pc_begin("XSH.mkdtemp.017");
	pc_require_space();
	pc_test();
	fd = mkostemp(t, O_APPEND);
	pc_expect_ok(fd, "mkostemp()");
	fl = fcntl(fd, F_GETFL);
	pc_check(fl != -1, "fcntl(F_GETFL) failed");
	pc_check((fl & O_APPEND) != 0, "O_APPEND is not among the status flags");
	pc_check((fl & O_ACCMODE) == O_RDWR, "access mode is not O_RDWR");
	pc_check(write(fd, "ab", 2) == 2, "write failed: %s", pc_errname(errno));
	pc_check(lseek(fd, 0, SEEK_SET) == 0, "lseek failed");
	pc_check(write(fd, "c", 1) == 1, "write failed: %s", pc_errname(errno));
	pc_check(lseek(fd, 0, SEEK_SET) == 0, "lseek failed");
	pc_check(read(fd, buf, 4) == 3 && memcmp(buf, "abc", 3) == 0, "the file does not hold \"abc\": the second write did not append");
	pc_pass();
}
