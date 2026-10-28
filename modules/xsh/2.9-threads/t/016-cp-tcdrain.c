/*
 * tcdrain(), a cancellation point, acts on a pending cancellation request
 * before it returns.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.2.9-threads.016
 *
 * The descriptor is the slave side of a pseudo-terminal with a byte of
 * output written to it. See XSH.2.9-threads.012. The test is not in use
 * when the system gives no pseudo-terminal.
 */
#include "pccp.h"
#include <fcntl.h>
#include <termios.h>

static volatile long sink;
static int slave;

static void c_tcdrain(void) { sink = tcdrain(slave); }

static const struct cpcase cases[] = {
	{ "tcdrain", NULL, c_tcdrain, NULL },
};

int main(void)
{
	char bad[256];
	int master, n;
	char *name;

	pc_begin("XSH.2.9-threads.016");
	guard(120);
	master = posix_openpt(O_RDWR | O_NOCTTY);
	if (master == -1)
		pc_notinuse("posix_openpt() failed: %s", pc_errname(errno));
	if (grantpt(master) != 0 || unlockpt(master) != 0)
		pc_notinuse("grantpt() or unlockpt() failed: %s", pc_errname(errno));
	name = ptsname(master);
	if (name == NULL)
		pc_notinuse("ptsname() failed");
	slave = open(name, O_RDWR | O_NOCTTY);
	if (slave == -1)
		pc_notinuse("opening the slave failed: %s", pc_errname(errno));
	NEED(isatty(slave), "the slave is not a terminal");
	sink = write(slave, "x", 1);
	pc_test();
	n = cp_run(cases, 1, CP_PENDING, bad, sizeof bad);
	pc_check(n == 0, "with a cancellation request pending and cancelability enabled the thread was not canceled in: %s", bad);
	pc_pass();
}
