/*
 * /dev/stdin, /dev/stdout and /dev/stderr, where they exist, are descriptors 0, 1 and 2.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION CONV.devnodes.006
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
	static const char *const names[] = { "stdin", "stdout", "stderr", NULL };
	char p[1024];
	struct stat a, b;
	int i, present = 0;

	pc_begin("CONV.devnodes.006");
	for (i = 0; names[i] != NULL; i++) {
		(void)cv_dev(p, sizeof p, names[i]);
		if (access(p, F_OK) == 0 || errno != ENOENT)
			present++;
	}
	if (present == 0)
		pc_notinuse("none of /dev/stdin, /dev/stdout, /dev/stderr exists; only some layout sources list them");
	pc_test();
	for (i = 0; names[i] != NULL; i++) {
		(void)cv_dev(p, sizeof p, names[i]);
		pc_check(stat(p, &a) == 0, "%s does not exist while another of the three does", p);
		pc_check(fstat(i, &b) == 0, "descriptor %d is closed", i);
		pc_check(a.st_dev == b.st_dev && a.st_ino == b.st_ino, "%s is not the file open as descriptor %d", p, i);
	}
	pc_pass();
}
