/*
 * /tmp is usable by every user.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION CONV.layout.003
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

	pc_begin("CONV.layout.003");
	(void)cv_rooted(p, sizeof p, "/tmp");
	pc_test();
	pc_check(stat(p, &st) == 0 && S_ISDIR(st.st_mode), "/tmp is not a directory");
	pc_check((st.st_mode & 0007) == 0007, "mode %04o: not readable, writable and searchable by everyone",
	    (unsigned)(st.st_mode & 07777));
	pc_pass();
}
