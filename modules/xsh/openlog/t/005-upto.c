/*
 * LOG_UPTO(p) covers the severities from LOG_EMERG through p in the listed order; each LOG_MASK is nonzero and different.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.openlog.005
 */
#include "pc.h"
#include <errno.h>
#include <fcntl.h>
#include <syslog.h>
#include <unistd.h>

#include "sl.h"

int main(void)
{
	int acc = 0, i, j;

	pc_begin("XSH.openlog.005");
	pc_test();
	for (i = 0; i < 8; i++) {
		pc_check(LOG_MASK(sl_sev[i]) != 0, "LOG_MASK of severity %d is zero", sl_sev[i]);
		for (j = i + 1; j < 8; j++)
			pc_check(LOG_MASK(sl_sev[i]) != LOG_MASK(sl_sev[j]),
			    "severities %d and %d have the same mask", sl_sev[i], sl_sev[j]);
		acc |= LOG_MASK(sl_sev[i]);
		pc_check(LOG_UPTO(sl_sev[i]) == acc, "LOG_UPTO of severity %d is 0x%x, expected 0x%x",
		    sl_sev[i], (unsigned)LOG_UPTO(sl_sev[i]), (unsigned)acc);
	}
	pc_check(LOG_UPTO(LOG_EMERG) == LOG_MASK(LOG_EMERG), "LOG_UPTO(LOG_EMERG) differs from LOG_MASK(LOG_EMERG)");
	pc_pass();
}
