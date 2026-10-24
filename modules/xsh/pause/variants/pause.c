/*
 * pause.c - conforming variants of pause(), for the variant checks (CI
 * only).
 *
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * A preload library; see core/pcvariant.h. Each variant says why the
 * standard allows it.
 */
#define _GNU_SOURCE 1
#include <errno.h>
#include <time.h>
#include <unistd.h>
#include "pcvariant.h"

/*
 * "late_return": pause() comes back 300 ms after the signal-catching
 * function returns. pause() DESCRIPTION says it returns after the handler
 * returns, with no bound on how soon (a slow scheduler); the result is
 * still -1 with EINTR.
 */
int pause(void)
{
	const char *v = variant_of("pause.");
	struct timespec ts = { 0, 300000000L };
	int r, e;
	REAL(pause);

	r = real_pause();
	e = errno;
	if (IS("late_return")) {
		while (nanosleep(&ts, &ts) == -1 && errno == EINTR)
			;
	}
	errno = e;
	return r;
}
