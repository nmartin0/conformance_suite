/*
 * pccp.h - harness for the cancellation point tests of XSH 2.9.5.2.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * A case is a function called by a thread of its own. Two modes:
 *
 *   CP_PENDING  the thread disables cancelability, the main thread makes a
 *               request, and the thread then enables cancelability and
 *               calls the function. The request is pending at the call, so
 *               the call must not return: the thread must be canceled in
 *               it (or before it returns), whether or not it would block.
 *   CP_BLOCKED  the thread calls a function that blocks with cancelability
 *               enabled; after 150 ms the main thread makes the request.
 *               The blocked call must be left by the cancellation.
 *
 * Between enabling cancelability and the call nothing else is called. A
 * function that returns means no cancellation was acted on there: the case
 * fails (the thread then calls pthread_testcancel() so that it ends). A
 * thread that neither returns nor is canceled within 10 s counts as having
 * blocked despite the request: the case fails.
 */
#ifndef PCCP_H
#define PCCP_H

#include "pcth.h"

enum { CP_PENDING = 0, CP_BLOCKED = 1 };

struct cpcase {
	const char *name;
	void (*setup)(void);	/* runs in the main thread before the case, or NULL */
	void (*fn)(void);	/* the call under test */
	void (*done)(void);	/* runs in the main thread after the case, or NULL */
};

/* State of the running case; the runner thread only touches these through flags. */
enum { F_READY = 0, F_GO = 1, F_ENTERED = 2, F_RETURNED = 3, F_CLEANED = 4 };

static int cp_mode;
static void (*cp_fn)(void);

static void cp_cleanup(void *arg)
{
	(void)arg;
	flag_set(F_CLEANED, 1);
}

static void *cp_runner(void *arg)
{
	int old;

	(void)arg;
	pthread_cleanup_push(cp_cleanup, NULL);
	if (cp_mode == CP_PENDING) {
		(void)pthread_setcancelstate(PTHREAD_CANCEL_DISABLE, &old);
		flag_set(F_READY, 1);
		while (!flag_get(F_GO))
			sched_yield();
		flag_set(F_ENTERED, 1);
		(void)pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, &old);
		cp_fn();
	} else {
		flag_set(F_READY, 1);
		flag_set(F_ENTERED, 1);
		cp_fn();
	}
	flag_set(F_RETURNED, 1);
	pthread_testcancel();
	pthread_cleanup_pop(0);
	return NULL;
}

/*
 * Runs the n cases in the given mode. Returns the number that failed and
 * puts their names, comma separated, in bad (size nbad).
 */
static inline int cp_run(const struct cpcase *cases, int n, int mode, char *bad, size_t nbad)
{
	int i, failed = 0, k;
	pthread_t t;
	void *st;
	size_t used = 0;
	const char *why;

	bad[0] = '\0';
	for (i = 0; i < n; i++) {
		int f;

		for (k = 0; k < 6; k++)
			flag_set(k, 0);
		cp_mode = mode;
		cp_fn = cases[i].fn;
		if (cases[i].setup != NULL)
			cases[i].setup();
		t = start(cp_runner, NULL);
		NEED(wait_flag(F_READY, 1, 10000), "%s: the thread did not start", cases[i].name);
		if (mode == CP_PENDING) {
			NEED(pthread_cancel(t) == 0, "%s: pthread_cancel failed", cases[i].name);
			flag_set(F_GO, 1);
		} else {
			sleep_ms(150);
			NEED(pthread_cancel(t) == 0, "%s: pthread_cancel failed", cases[i].name);
		}
		for (f = 0; f < 5000; f++) {
			if (flag_get(F_CLEANED) || flag_get(F_RETURNED))
				break;
			sleep_ms(1);
		}
		why = NULL;
		if (flag_get(F_RETURNED)) {
			why = "returned";
			(void)join_within(t, 5000, &st);
		} else if (!flag_get(F_CLEANED)) {
			why = "blocked";
		} else if (!join_within(t, 5000, &st)) {
			why = "not ended";
		} else if (st != PTHREAD_CANCELED) {
			why = "bad status";
		}
		if (why != NULL) {
			int w = snprintf(bad + used, nbad - used, "%s%s (%s)", failed ? ", " : "", cases[i].name, why);

			if (w > 0 && (size_t)w < nbad - used)
				used += (size_t)w;
			failed++;
		}
		if (cases[i].done != NULL)
			cases[i].done();
	}
	return failed;
}

#endif
