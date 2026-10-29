/*
 * Threads that are alive together have separate copies of a thread-local object.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION ABI.i386.threads.010
 */
#include "../../../../check.h"
#include <pthread.h>
#include <string.h>

#ifndef ABI_ARCH_I386
ABI_NOT_THIS_ARCH("ABI.i386.threads.010")
#else
#define N 4
static _Thread_local int tl_val = 7;
static _Thread_local int tl_zero;
static pthread_barrier_t bar;

struct slot { int id; int val_back; int zero_back; void *addr; void *zaddr; };

static void *tbody(void *arg)
{
	struct slot *s = arg;

	tl_val = 100 + s->id;
	tl_zero = 200 + s->id;
	s->addr = &tl_val;
	s->zaddr = &tl_zero;
	pthread_barrier_wait(&bar);
	s->val_back = tl_val;
	s->zero_back = tl_zero;
	pthread_barrier_wait(&bar);
	return NULL;
}
int main(void)
{
	pc_begin("ABI.i386.threads.010");
	pthread_t t[N];
	struct slot s[N];
	int i, j;

	pc_test();
	if (pthread_barrier_init(&bar, NULL, N + 1) != 0)
		pc_unresolved("pthread_barrier_init failed");
	for (i = 0; i < N; i++) {
		s[i].id = i;
		if (pthread_create(&t[i], NULL, tbody, &s[i]) != 0)
			pc_unresolved("pthread_create failed");
	}
	tl_val = 5000;
	tl_zero = 6000;
	pthread_barrier_wait(&bar);     /* every thread has written its own value */
	pthread_barrier_wait(&bar);     /* and read it back */
	for (i = 0; i < N; i++)
		pthread_join(t[i], NULL);
	for (i = 0; i < N; i++) {
		pc_check(s[i].val_back == 100 + i, "thread %d read back tl_val = %d after the others wrote theirs", i, s[i].val_back);
		pc_check(s[i].zero_back == 200 + i, "thread %d read back tl_zero = %d after the others wrote theirs", i, s[i].zero_back);
		pc_check(s[i].addr != (void *)&tl_val, "thread %d has the same address for tl_val as the main thread", i);
		for (j = i + 1; j < N; j++) {
			pc_check(s[i].addr != s[j].addr, "threads %d and %d have the same address for tl_val", i, j);
			pc_check(s[i].zaddr != s[j].zaddr, "threads %d and %d have the same address for tl_zero", i, j);
		}
	}
	pc_check(tl_val == 5000 && tl_zero == 6000, "the main thread's copies changed (%d, %d)", tl_val, tl_zero);
	pc_pass();
}
#endif
