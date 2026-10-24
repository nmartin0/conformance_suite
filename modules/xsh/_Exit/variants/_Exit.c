/*
 * _Exit.c - conforming variants of _exit() and _Exit(), for the variant
 * checks (CI only).
 *
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * A preload library; see core/pcvariant.h. Each variant says why the
 * standard allows it.
 */
#define _GNU_SOURCE 1
#include <errno.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include "pcvariant.h"

/*
 * "late_detach": the process's shared-memory attachments are released
 * half a second after its status has become available. The consequences
 * of process termination in XSH _Exit() are a list with no order among
 * them and no time bound, so a parent that has collected the status may
 * still see the old shm_nattch for a while. Planted with a helper that
 * inherits the attachments (XSH fork()), keeps no descriptor, and ends
 * after 500 ms.
 */
static void linger(void)
{
	const char *v = variant_of("_Exit.");

	if (IS("late_detach") && fork() == 0) {
		struct timespec ts = { 0, 500000000 };
		long fd, max = sysconf(_SC_OPEN_MAX);

		if (max < 0 || max > 65536)
			max = 65536;
		for (fd = 0; fd < max; fd++)
			(void)close((int)fd);
		while (nanosleep(&ts, &ts) == -1 && errno == EINTR)
			;
	}
}

void _exit(int status)
{
	REAL(_exit);

	linger();
	real__exit(status);
	for (;;)
		;
}

void _Exit(int status)
{
	REAL(_Exit);

	linger();
	real__Exit(status);
	for (;;)
		;
}
