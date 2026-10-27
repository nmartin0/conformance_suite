/*
 * pwrite() on a FIFO fails with ESPIPE.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.pwrite.008
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
	int r, w;

	pc_begin("XSH.pwrite.008");
	if (mkfifo("ff", 0600) != 0)
		pc_unresolved("mkfifo failed: %s", pc_errname(errno));
	r = open("ff", O_RDONLY | O_NONBLOCK);
	pc_check(r >= 0, "cannot open the FIFO for reading");
	w = open("ff", O_WRONLY);
	pc_check(w >= 0, "cannot open the FIFO for writing");
	pc_test();
	pc_expect_err(pwrite(w, "x", 1, 0), ESPIPE, "pwrite() on a FIFO");
	pc_pass();
}
