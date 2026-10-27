/*
 * syslog() calls rejected by the mask return normally and leave the mask alone.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.openlog.006
 */
#include "pc.h"
#include <errno.h>
#include <fcntl.h>
#include <syslog.h>
#include <unistd.h>

#include "sl.h"

int main(void)
{
	int i, m;

	pc_begin("XSH.openlog.006");
	alarm(60);
	pc_test();
	(void)setlogmask(LOG_MASK(LOG_EMERG));
	errno = ENOENT;
	for (i = 1; i < 8; i++) {
		syslog(sl_sev[i], "pcheck masked message %d", i);
		syslog(sl_sev[i] | LOG_LOCAL3, "pcheck masked message %s %d %m", "with facility", i);
	}
	syslog(LOG_DEBUG, "%s", "pcheck masked");
	m = setlogmask(0);
	pc_check(m == LOG_MASK(LOG_EMERG), "the mask changed to 0x%x", (unsigned)m);
	pc_pass();
}
