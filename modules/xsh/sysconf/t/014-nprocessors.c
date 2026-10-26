/*
 * sysconf() counts of execution units: at least one available, and no
 * more available than can be made available.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.sysconf.014
 * MUTANT    sysconf.onln_zero
 * MUTANT    sysconf.conf_zero
 */
#include "pc.h"
#include <errno.h>
#include <unistd.h>

int main(void)
{
	long on, conf;

	pc_begin("XSH.sysconf.014");
#if !defined(_SC_NPROCESSORS_ONLN) || !defined(_SC_NPROCESSORS_CONF)
	pc_unresolved("<unistd.h> does not define _SC_NPROCESSORS_ONLN and _SC_NPROCESSORS_CONF");
#else
	pc_test();
	on = sysconf(_SC_NPROCESSORS_ONLN);
	conf = sysconf(_SC_NPROCESSORS_CONF);
	pc_check(on >= 1, "sysconf(_SC_NPROCESSORS_ONLN) returned %ld, but this thread is running on one", on);
	pc_check(conf >= on, "sysconf(_SC_NPROCESSORS_CONF) returned %ld, fewer than the %ld available just before",
		 conf, on);
	pc_pass();
#endif
}
