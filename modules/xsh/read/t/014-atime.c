/*
 * read() updates the access time.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.read.014
 * MUTANT    read.no_atime
 */
#include "pc.h"
#include <errno.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>

int main(void)
{
	struct timespec ts[2];
	struct stat st;
	char buf[4];
	int fd;

	pc_begin("XSH.read.014");
	pc_make_file("f", "0123456789");
	pc_check(stat("f", &st) == 0, "stat failed");
	ts[0].tv_sec = st.st_mtim.tv_sec - 100;
	ts[0].tv_nsec = 0;
	ts[1].tv_sec = 0;
	ts[1].tv_nsec = UTIME_OMIT;
	pc_check(utimensat(AT_FDCWD, "f", ts, 0) == 0, "cannot set the access time back");
	fd = open("f", O_RDONLY);
	pc_check(fd >= 0, "cannot open f");
	pc_test();
	pc_check(read(fd, buf, sizeof buf) == 4, "read failed");
	pc_check(fstat(fd, &st) == 0, "fstat failed");
	pc_check(st.st_atim.tv_sec > ts[0].tv_sec, "the access time was not updated");
	pc_pass();
}
