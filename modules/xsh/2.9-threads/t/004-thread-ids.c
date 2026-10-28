/*
 * Thread IDs are distinct among the threads of a process and stay usable
 * until the thread has terminated and is no longer joinable.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.2.9-threads.004
 */
#include "pcth.h"

#define NT 8

static pthread_t ids[NT];
static pthread_t selves[NT];
static int idx[NT];

static void *live(void *a)
{
	int i = *(int *)a;

	selves[i] = pthread_self();
	flag_add(0, 1);
	while (!flag_get(1))
		sleep_ms(1);
	return (void *)(long)(100 + i);
}

static void *quick(void *a)
{
	(void)a;
	return (void *)(long)77;
}

int main(void)
{
	pthread_t q;
	int i, j;
	void *st;

	pc_begin("XSH.2.9-threads.004");
	guard(60);
	pc_test();
	for (i = 0; i < NT; i++) {
		idx[i] = i;
		NEED(pthread_create(&ids[i], NULL, live, &idx[i]) == 0, "pthread_create failed");
	}
	NEED(wait_flag(0, NT, 10000), "the threads did not all start");
	for (i = 0; i < NT; i++) {
		pc_check(pthread_equal(ids[i], selves[i]) != 0, "thread %d: the ID pthread_create() gave is not the ID pthread_self() gives in it", i);
		pc_check(pthread_equal(ids[i], pthread_self()) == 0, "thread %d has the ID of the main thread", i);
		for (j = i + 1; j < NT; j++)
			pc_check(pthread_equal(ids[i], ids[j]) == 0, "live threads %d and %d have equal IDs", i, j);
	}
	flag_set(1, 1);
	for (i = 0; i < NT; i++) {
		NEED(pthread_join(ids[i], &st) == 0, "pthread_join of thread %d failed", i);
		pc_check(st == (void *)(long)(100 + i), "thread %d returned a different value", i);
	}
	/* A thread that has terminated but has not been joined still has a usable ID. */
	NEED(pthread_create(&q, NULL, quick, NULL) == 0, "pthread_create failed");
	sleep_ms(300);
	i = pthread_join(q, &st);
	pc_check(i == 0, "pthread_join() of a terminated thread that was not joined before returned %s", pc_errname(i));
	pc_check(st == (void *)(long)77, "the terminated thread's value was lost");
	pc_pass();
}
