/*
 * /usr/bin/env is an executable file.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION CONV.layout.010
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
	char p[1024];
	struct stat st;

	pc_begin("CONV.layout.010");
	(void)cv_rooted(p, sizeof p, "/usr/bin/env");
	pc_test();
	pc_check(stat(p, &st) == 0, "/usr/bin/env does not exist");
	pc_check(S_ISREG(st.st_mode), "/usr/bin/env does not resolve to a regular file");
	pc_check((st.st_mode & 0111) == 0111, "mode %04o: not executable by everyone", (unsigned)(st.st_mode & 07777));
	pc_pass();
}
