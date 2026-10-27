/*
 * sl.h - helpers shared by the openlog tests.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * Every syslog() call the tests make is rejected by a log mask that holds
 * only LOG_EMERG (which the tests never send), so nothing reaches a log.
 */
#ifndef SL_H
#define SL_H

#include "pc.h"
#include <fcntl.h>
#include <syslog.h>
#include <unistd.h>

#define SL_FDS 1024

static const int sl_sev[8] = {
	LOG_EMERG, LOG_ALERT, LOG_CRIT, LOG_ERR, LOG_WARNING, LOG_NOTICE, LOG_INFO, LOG_DEBUG
};

/* open[fd] is the descriptor flags, or -1 if fd is not open. */
static inline int sl_scan(int *open_flags)
{
	long lim = sysconf(_SC_OPEN_MAX);
	int fd, n = 0;

	if (lim < 0 || lim > SL_FDS)
		lim = lim < 0 ? 256 : SL_FDS;
	for (fd = 0; fd < SL_FDS; fd++) {
		open_flags[fd] = (fd < lim) ? fcntl(fd, F_GETFD) : -1;
		if (open_flags[fd] != -1)
			n++;
	}
	return n;
}

#endif
