/*
 * mq_open() with O_CREAT|O_EXCL is atomic: of several threads naming one queue exactly one creates it.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.mq_open.017
 */
#include "mqh.h"
#include <stdatomic.h>

#define NAMES 40
#define THREADS 3

static char names[NAMES][80];
static atomic_int wins[NAMES], losses[NAMES], other[NAMES], nospace;
static pthread_barrier_t bar;

static void *worker(void *arg)
{
	struct mq_attr at;
	int i;

	(void)arg;
	memset(&at, 0, sizeof at);
	at.mq_maxmsg = 2;
	at.mq_msgsize = 8;
	pthread_barrier_wait(&bar);
	for (i = 0; i < NAMES; i++) {
		mqd_t q = mq_open(names[i], O_RDWR | O_CREAT | O_EXCL, (mode_t)0600, &at);

		if (q != BADQ) {
			atomic_fetch_add(&wins[i], 1);
			(void)mq_close(q);
		} else if (errno == EEXIST) {
			atomic_fetch_add(&losses[i], 1);
		} else if (errno == ENOSPC) {
			atomic_fetch_add(&nospace, 1);
		} else {
			atomic_fetch_add(&other[i], 1);
		}
	}
	return NULL;
}

int main(void)
{
	pthread_t t[THREADS];
	char tag[16];
	int i;

	pc_begin("XSH.mq_open.017");
	for (i = 0; i < NAMES; i++) {
		snprintf(tag, sizeof tag, "e%d", i);
		snprintf(names[i], sizeof names[i], "%s", mq_name(tag));
	}
	NEED(pthread_barrier_init(&bar, NULL, THREADS) == 0, "pthread_barrier_init failed");
	pc_test();
	for (i = 0; i < THREADS; i++)
		NEED(pthread_create(&t[i], NULL, worker, NULL) == 0, "pthread_create failed");
	for (i = 0; i < THREADS; i++)
		NEED(pthread_join(t[i], NULL) == 0, "pthread_join failed");
	NEED(atomic_load(&nospace) == 0, "the system has no room for %d queues", NAMES);
	for (i = 0; i < NAMES; i++) {
		pc_check(atomic_load(&other[i]) == 0, "%s: mq_open failed with an error other than EEXIST", names[i]);
		pc_check(atomic_load(&wins[i]) == 1, "%s: %d threads created it", names[i], atomic_load(&wins[i]));
		pc_check(atomic_load(&losses[i]) == THREADS - 1, "%s: %d threads got EEXIST", names[i], atomic_load(&losses[i]));
	}
	pc_pass();
}
