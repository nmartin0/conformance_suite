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
	pc_check(write(w, "xy", 2) == 2, "write to the FIFO failed");
	pc_test();
	pc_expect_ok(close(w), "close(writer)");
	/* Not the last close: the data must still be there. */
	n = read(r, &c, 1);
	pc_check(n == 1 && c == 'x',
	    "data discarded when the writer closed while a reader was open");
	pc_expect_ok(close(r), "close(reader)");
	r = open("p", O_RDONLY | O_NONBLOCK);
	if (r < 0)
		pc_unresolved("cannot reopen the FIFO (%s)", pc_errname(errno));
	n = read(r, &c, 1);
	pc_check(n != 1, "data left at the last close is still in the FIFO");
	pc_pass();
}
