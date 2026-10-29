/*
 * /dev/fd/N opens the process's descriptor N.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION CONV.devnodes.005
 */
#include <sys/stat.h>
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "pc.h"
#include "../../../../lib/cv.h"

int main(void)
{
	char p[1024], q[1100];
	struct stat a, b;
	int fd, d, n;

	pc_begin("CONV.devnodes.005");
	fd = open("fdtest", O_RDWR | O_CREAT | O_EXCL, 0600);
	pc_check(fd >= 0, "cannot create a file: errno %d", errno);
	(void)cv_dev(p, sizeof p, "fd");
	pc_test();
	pc_check(cv_isdir(p), "%s is not a directory", p);
	for (n = 0; n < 3; n++) {
		(void)snprintf(q, sizeof q, "%s/%d", p, n);
		d = open(q, O_RDONLY | O_NONBLOCK | O_NOCTTY);
		pc_check(d >= 0, "cannot open %s: errno %d", q, errno);
		pc_check(fstat(d, &a) == 0 && fstat(n, &b) == 0, "fstat failed");
		pc_check(a.st_dev == b.st_dev && a.st_ino == b.st_ino,
		    "%s is not the file open as descriptor %d", q, n);
		(void)close(d);
	}
	(void)snprintf(q, sizeof q, "%s/%d", p, fd);
	d = open(q, O_RDWR);
	pc_check(d >= 0, "cannot open %s: errno %d", q, errno);
	pc_check(fstat(d, &a) == 0 && fstat(fd, &b) == 0 && a.st_dev == b.st_dev && a.st_ino == b.st_ino,
	    "%s is not the file open as descriptor %d", q, fd);
	pc_check(write(d, "x", 1) == 1 && lseek(fd, 0, SEEK_END) == 1, "a write through %s is not seen on descriptor %d", q, fd);
	(void)close(d);
	pc_pass();
}
