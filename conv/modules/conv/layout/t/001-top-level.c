/*
 * The top-level directories every layout description lists.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION CONV.layout.001
 */
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "pc.h"
#include "../../../../lib/cv.h"

int main(void)
{
	static const char *const dirs[] = {
		"/", "/bin", "/sbin", "/dev", "/etc", "/tmp", "/usr", "/var", "/var/tmp", "/mnt", NULL
	};
	char p[1024];
	struct stat st;
	int i;

	pc_begin("CONV.layout.001");
	pc_test();
	for (i = 0; dirs[i] != NULL; i++) {
		(void)cv_rooted(p, sizeof p, dirs[i]);
		pc_check(stat(p, &st) == 0, "%s does not exist (or is a dangling link)", dirs[i]);
		pc_check(S_ISDIR(st.st_mode), "%s does not resolve to a directory", dirs[i]);
	}
	pc_pass();
}
