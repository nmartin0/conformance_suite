/*
 * closelog() closes the descriptors openlog() allocated.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.openlog.008
 */
#include "pc.h"
#include <errno.h>
#include <fcntl.h>
#include <syslog.h>
#include <unistd.h>

#include "sl.h"

int main(void)
{
	static int before[SL_FDS], mid[SL_FDS], after[SL_FDS];
	int fd, found = 0;

	pc_begin("XSH.openlog.008");
	pc_test();
	(void)setlogmask(LOG_MASK(LOG_EMERG));
	sl_scan(before);
	openlog("pcheck", LOG_NDELAY, LOG_USER);
	sl_scan(mid);
	for (fd = 0; fd < SL_FDS; fd++)
		if (before[fd] == -1 && mid[fd] != -1)
			found++;
	closelog();
	sl_scan(after);
	for (fd = 0; fd < SL_FDS; fd++)
		pc_check((after[fd] == -1) == (before[fd] == -1), "descriptor %d is %s after closelog", fd,
		    after[fd] == -1 ? "closed" : "still open");
	closelog();
	if (found == 0)
		pc_notinuse("openlog allocated no descriptor");
	pc_pass();
}
