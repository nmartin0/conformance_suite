/*
 * /tmp has the sticky bit.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION CONV.layout.004
 */
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "pc.h"

/* S_ISVTX is an XSI name; the bit has always been 01000 */
#ifdef S_ISVTX
#define CV_STICKY S_ISVTX
#else
#define CV_STICKY 01000
#endif
#include "../../../../lib/cv.h"

int main(void)
{
	char p[1024];
	struct stat st;

	pc_begin("CONV.layout.004");
	(void)cv_rooted(p, sizeof p, "/tmp");
	pc_test();
	pc_check(stat(p, &st) == 0 && S_ISDIR(st.st_mode), "/tmp is not a directory");
	pc_check((st.st_mode & CV_STICKY) != 0, "mode %04o: the sticky bit is not set",
	    (unsigned)(st.st_mode & 07777));
	pc_pass();
}
