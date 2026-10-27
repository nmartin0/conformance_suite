/*
 * setlogmask(0) returns the current mask and does not change it.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.openlog.002
 */
#include "pc.h"
#include <errno.h>
#include <fcntl.h>
#include <syslog.h>
#include <unistd.h>

#include "sl.h"

int main(void)
{
	int a, b, c;

	pc_begin("XSH.openlog.002");
	pc_test();
	a = setlogmask(0);
	b = setlogmask(0);
	pc_check(a == b, "two setlogmask(0) calls returned 0x%x and 0x%x", (unsigned)a, (unsigned)b);
	(void)setlogmask(LOG_MASK(LOG_ERR));
	a = setlogmask(0);
	b = setlogmask(0);
	c = setlogmask(0);
	pc_check(a == LOG_MASK(LOG_ERR), "setlogmask(0) returned 0x%x after setting LOG_MASK(LOG_ERR)",
	    (unsigned)a);
	pc_check(b == a && c == a, "setlogmask(0) changed the mask");
	pc_pass();
}
