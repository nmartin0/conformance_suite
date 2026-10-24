/*
 * sigtimedwait.c - conforming variants of sigtimedwait() and sigwaitinfo(),
 * for the variant checks (CI only).
 *
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * A preload library; see core/pcvariant.h. Each variant says why the
 * standard allows it.
 */
#define _GNU_SOURCE 1
#include <errno.h>
#include <signal.h>
#include <string.h>
#include <time.h>
#include "pcvariant.h"

#define LATE_NS 400000000L

static void sleep_late(void)
{
	struct timespec ts = { 0, LATE_NS };

	while (nanosleep(&ts, &ts) == -1 && errno == EINTR)
		;
}

/*
 * "junk_fields": the members a sigwait function leaves undefined hold
 * garbage. XSH 2.4.3 defines si_value only for SI_QUEUE, SI_TIMER,
 * SI_ASYNCIO and SI_MESGQ (and sigtimedwait() DESCRIPTION leaves it
 * undefined when no value was queued); for SI_USER and the other codes of
 * a sending process only si_pid and si_uid are defined. Only si_value is
 * scribbled on, since on this layout it does not share storage with a
 * member those codes define.
 */
static void junk(siginfo_t *info)
{
	int c = info->si_code;

	if (c == SI_QUEUE || c == SI_TIMER || c == SI_ASYNCIO || c == SI_MESGQ)
		return;
	if (c == SI_USER || c <= 0)
		memset(&info->si_value, 0x5a, sizeof info->si_value);
}

/*
 * "sluggish": a wait that has to block comes back late, as on a system
 * with a coarse timer and a slow scheduler. sigtimedwait() DESCRIPTION
 * only says the thread waits for the interval (a lower bound) or until a
 * signal in the set becomes pending, with no bound on how soon it then
 * returns; a timeout here ends 400 ms after the interval, and a wakeup
 * by a signal returns 400 ms after the signal. A signal already pending,
 * or a zero timeout, still returns at once, as DESCRIPTION requires.
 */
int sigtimedwait(const sigset_t *set, siginfo_t *info, const struct timespec *to)
{
	const char *v = variant_of("sigtimedwait.");
	struct timespec zero = { 0, 0 }, ext;
	int r, e;
	REAL(sigtimedwait);

	if (IS("sluggish") && to != NULL && to->tv_nsec >= 0 && to->tv_nsec < 1000000000L) {
		r = real_sigtimedwait(set, info, &zero);
		if (r > 0 || (to->tv_sec == 0 && to->tv_nsec == 0))
			return r;
		ext = *to;
		ext.tv_nsec += LATE_NS;
		if (ext.tv_nsec >= 1000000000L) {
			ext.tv_nsec -= 1000000000L;
			ext.tv_sec++;
		}
		r = real_sigtimedwait(set, info, &ext);
		e = errno;
		if (r > 0)
			sleep_late();
		errno = e;
		return r;
	}
	r = real_sigtimedwait(set, info, to);
	if (r > 0 && info != NULL && IS("junk_fields"))
		junk(info);
	return r;
}

int sigwaitinfo(const sigset_t *set, siginfo_t *info)
{
	const char *v = variant_of("sigtimedwait.");
	struct timespec zero = { 0, 0 };
	int r, e;
	REAL(sigwaitinfo);
	REAL(sigtimedwait);

	if (IS("sluggish")) {
		r = real_sigtimedwait(set, info, &zero);
		if (r > 0)
			return r;
		r = real_sigwaitinfo(set, info);
		e = errno;
		if (r > 0)
			sleep_late();
		errno = e;
		return r;
	}
	r = real_sigwaitinfo(set, info);
	if (r > 0 && info != NULL && IS("junk_fields"))
		junk(info);
	return r;
}
