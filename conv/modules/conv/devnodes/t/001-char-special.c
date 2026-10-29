/*
 * The standard device nodes are character special files.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION CONV.devnodes.001
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
	static const char *const names[] = { "null", "zero", "tty", NULL };
	char p[1024];
	struct stat st;
	int i;

	pc_begin("CONV.devnodes.001");
	pc_test();
	for (i = 0; names[i] != NULL; i++) {
		(void)cv_dev(p, sizeof p, names[i]);
		pc_check(stat(p, &st) == 0, "%s does not exist", p);
		pc_check(S_ISCHR(st.st_mode), "%s is not a character special file", p);
	}
	pc_pass();
}
