/*
 * A descriptor that openlog() allocates has FD_CLOEXEC set.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.openlog.007
 */
#include "pc.h"
#include <errno.h>
#include <fcntl.h>
#include <syslog.h>
#include <unistd.h>

#include "sl.h"

int main(void)
{
	static int before[SL_FDS], after[SL_FDS];
	int fd, found = 0;

	pc_begin("XSH.openlog.007");
	pc_test();
	(void)setlogmask(LOG_MASK(LOG_EMERG));
	sl_scan(before);
	openlog("pcheck", LOG_NDELAY | LOG_PID, LOG_USER);
	sl_scan(after);
	for (fd = 0; fd < SL_FDS; fd++) {
		if (before[fd] == -1 && after[fd] != -1) {
			found++;
			pc_check((after[fd] & FD_CLOEXEC) != 0, "descriptor %d allocated by openlog lacks FD_CLOEXEC", fd);
		}
	}
	closelog();
	if (found == 0)
		pc_notinuse("openlog allocated no descriptor");
	pc_pass();
}
