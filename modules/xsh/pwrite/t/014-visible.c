/*
 * pwrite() data is seen by a read through another descriptor and replaced by a later pwrite().
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.pwrite.014
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
	char buf[8];
	int w, r;

	pc_begin("XSH.pwrite.014");
	pc_require_space();
	w = open("f", O_RDWR | O_CREAT, 0600);
	pc_check(w >= 0, "cannot create f");
	r = open("f", O_RDONLY);
	pc_check(r >= 0, "cannot reopen f");
	pc_test();
	memset(buf, 0, sizeof buf);
	pc_check(pwrite(w, "hello", 5, 0) == 5, "pwrite() did not return 5");
	pc_check(read(r, buf, 5) == 5 && memcmp(buf, "hello", 5) == 0, "read() through the other descriptor did not return the data");
	pc_check(pwrite(w, "HE", 2, 0) == 2, "second pwrite() did not return 2");
	pc_check(pread(r, buf, 5, 0) == 5 && memcmp(buf, "HEllo", 5) == 0, "the later pwrite() did not replace the data");
	pc_pass();
}
