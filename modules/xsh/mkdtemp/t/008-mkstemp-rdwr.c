/*
 * The descriptor of mkstemp() is open for reading and writing.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.mkdtemp.008
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

	pc_begin("XSH.mkdtemp.008");
	pc_require_space();
	pc_test();
	fd = mkstemp(t);
	pc_expect_ok(fd, "mkstemp()");
	fl = fcntl(fd, F_GETFL);
	pc_check(fl != -1, "fcntl(F_GETFL) failed");
	pc_check((fl & O_ACCMODE) == O_RDWR, "access mode is not O_RDWR");
	pc_check(write(fd, "hello", 5) == 5, "write failed: %s", pc_errname(errno));
	pc_check(lseek(fd, 0, SEEK_SET) == 0, "lseek failed");
	pc_check(read(fd, buf, 5) == 5 && memcmp(buf, "hello", 5) == 0, "the bytes written were not read back");
	pc_pass();
}
