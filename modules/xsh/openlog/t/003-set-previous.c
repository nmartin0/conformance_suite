/*
 * setlogmask() sets the mask and returns the previous one.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.openlog.003
 */
#include "pc.h"
#include <errno.h>
#include <fcntl.h>
#include <syslog.h>
#include <unistd.h>

#include "sl.h"

int main(void)
{
	int m0, p;

	pc_begin("XSH.openlog.003");
	pc_test();
	m0 = setlogmask(0);
	p = setlogmask(LOG_MASK(LOG_ERR));
	pc_check(p == m0, "first call returned 0x%x, the mask was 0x%x", (unsigned)p, (unsigned)m0);
	p = setlogmask(LOG_UPTO(LOG_WARNING));
	pc_check(p == LOG_MASK(LOG_ERR), "second call returned 0x%x, expected LOG_MASK(LOG_ERR)", (unsigned)p);
	p = setlogmask(LOG_MASK(LOG_DEBUG) | LOG_MASK(LOG_INFO));
	pc_check(p == LOG_UPTO(LOG_WARNING), "third call returned 0x%x, expected LOG_UPTO(LOG_WARNING)",
	    (unsigned)p);
	p = setlogmask(m0);
	pc_check(p == (LOG_MASK(LOG_DEBUG) | LOG_MASK(LOG_INFO)), "fourth call returned 0x%x", (unsigned)p);
	p = setlogmask(0);
	pc_check(p == m0, "the restored mask is 0x%x, expected 0x%x", (unsigned)p, (unsigned)m0);
	pc_pass();
}
