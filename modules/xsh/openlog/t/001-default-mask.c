/*
 * The default log mask allows every priority.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.openlog.001
 */
#include "pc.h"
#include <errno.h>
#include <fcntl.h>
#include <syslog.h>
#include <unistd.h>

#include "sl.h"

int main(void)
{
	int m, prev, q, i;

	pc_begin("XSH.openlog.001");
	pc_test();
	m = setlogmask(0);
	for (i = 0; i < 8; i++)
		pc_check((m & LOG_MASK(sl_sev[i])) != 0, "severity %d is not in the default mask 0x%x",
		    sl_sev[i], (unsigned)m);
	/* the query is a working mask function, not one that answers "everything" whatever is set:
	   setting a mask returns the one before, and the new mask is then what a query shows */
	prev = setlogmask(LOG_MASK(LOG_EMERG));
	pc_check(prev == m, "setlogmask() returned 0x%x as the previous mask, the query had shown 0x%x", (unsigned)prev, (unsigned)m);
	q = setlogmask(0);
	pc_check((q & LOG_MASK(LOG_EMERG)) != 0, "after setting LOG_MASK(LOG_EMERG) the mask 0x%x lacks it", (unsigned)q);
	for (i = 1; i < 8; i++)
		pc_check((q & LOG_MASK(sl_sev[i])) == 0, "after setting LOG_MASK(LOG_EMERG) severity %d is still in the mask 0x%x",
		    sl_sev[i], (unsigned)q);
	pc_pass();
}
