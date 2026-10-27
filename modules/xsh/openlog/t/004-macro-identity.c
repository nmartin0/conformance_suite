/*
 * The two mask expressions the page shows as equal are equal; the macros give int.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.openlog.004
 */
#include "pc.h"
#include <errno.h>
#include <fcntl.h>
#include <syslog.h>
#include <unistd.h>

#include "sl.h"

int main(void)
{
	int pri = LOG_ERR;
	int a, b;

	pc_begin("XSH.openlog.004");
	pc_test();
	a = LOG_UPTO(LOG_WARNING) | LOG_MASK(LOG_DEBUG);
	b = LOG_UPTO(LOG_DEBUG) & ~(LOG_MASK(LOG_NOTICE) | LOG_MASK(LOG_INFO));
	pc_check(a == b, "0x%x and 0x%x differ", (unsigned)a, (unsigned)b);
	pc_check(_Generic(LOG_MASK(pri), int: 1, default: 0), "LOG_MASK(pri) is not an int");
	pc_check(_Generic(LOG_UPTO(pri), int: 1, default: 0), "LOG_UPTO(pri) is not an int");
	pc_pass();
}
