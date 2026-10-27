/*
 * An open file can still be read after remove().
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.remove.005
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
	struct stat st;
	char buf[8];
	int fd;

	pc_begin("XSH.remove.005");
	pc_make_file("f", "still here");
	fd = open("f", O_RDONLY);
	pc_check(fd >= 0, "cannot open f");
	pc_test();
	pc_check(remove("f") == 0, "remove() failed: %s", pc_errname(errno));
	pc_check(lstat("f", &st) == -1 && errno == ENOENT, "the name still exists");
	memset(buf, 0, sizeof buf);
	pc_check(read(fd, buf, 5) == 5 && memcmp(buf, "still", 5) == 0, "the open descriptor no longer reads the file");
	pc_pass();
}
