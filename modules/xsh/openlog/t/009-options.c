/*
 * openlog() accepts every option and facility constant, repeatedly, and closelog() can follow with or without it.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.openlog.009
 */
#include "pc.h"
#include <errno.h>
#include <fcntl.h>
#include <syslog.h>
#include <unistd.h>

#include "sl.h"

int main(void)
{
	static const int opts[] = {
		0, LOG_PID, LOG_CONS, LOG_NDELAY, LOG_ODELAY, LOG_NOWAIT,
		LOG_PID | LOG_CONS | LOG_NOWAIT | LOG_NDELAY
	};
	static const int facs[] = {
		LOG_KERN, LOG_USER, LOG_MAIL, LOG_NEWS, LOG_UUCP, LOG_DAEMON, LOG_AUTH, LOG_CRON,
		LOG_LPR, LOG_LOCAL0, LOG_LOCAL1, LOG_LOCAL2, LOG_LOCAL3, LOG_LOCAL4, LOG_LOCAL5,
		LOG_LOCAL6, LOG_LOCAL7
	};
	char ident[16] = "pcheck";
	size_t i, j;

	pc_begin("XSH.openlog.009");
	alarm(60);
	pc_test();
	closelog();
	(void)setlogmask(LOG_MASK(LOG_EMERG));
	for (i = 0; i < sizeof opts / sizeof opts[0]; i++) {
		for (j = 0; j < sizeof facs / sizeof facs[0]; j++) {
			openlog(ident, opts[i], facs[j]);
			openlog(ident, opts[i], facs[j]);
			syslog(LOG_DEBUG, "pcheck masked %d", (int)j);
			closelog();
		}
	}
	closelog();
	pc_pass();
}
