/*
 * Data left in a FIFO is discarded when every descriptor is closed.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.close.006
 * MUTANT    close.noop
 */
#include "pc.h"
#include <errno.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>

int main(void)
{
	int r, w;
	ssize_t n;
	char c;

	pc_begin("XSH.close.006");
	pc_check(mkfifo("p", 0600) == 0, "mkfifo failed");
	r = open("p", O_RDONLY | O_NONBLOCK);
	pc_check(r >= 0, "cannot open the FIFO for reading");
	w = open("p", O_WRONLY);
	pc_check(w >= 0, "cannot open the FIFO for writing");
	pc_check(write(w, "x", 1) == 1, "write to the FIFO failed");
	pc_test();
	pc_expect_ok(close(w), "close(writer)");
	pc_expect_ok(close(r), "close(reader)");
	r = open("p", O_RDONLY | O_NONBLOCK);
	pc_check(r >= 0, "cannot reopen the FIFO");
	n = read(r, &c, 1);
	pc_check(n != 1, "data written before the last close is still in the FIFO");
	pc_check(n == 0, "read() with no writer gave %ld (%s), expected 0",
	    (long)n, n < 0 ? pc_errname(errno) : "-");
	pc_pass();
}
