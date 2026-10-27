/*
 * One attributes object works in simultaneous pthread_create() calls.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.pthread_attr_init.003
 */
#include "pc.h"
#include <errno.h>
#include <pthread.h>
#include <stdatomic.h>
#include <stdint.h>

#define NEED(cond, ...) do { if (!(cond)) pc_unresolved(__VA_ARGS__); } while (0)
#define CREATORS 4
#define ROUNDS 25

static pthread_attr_t attr;
static pthread_barrier_t gate;
static atomic_int ran;

struct creator {
	pthread_t thread;
	int id;
	int bad;	/* 0 fine, else the first problem: 1 create error, 2 join error, 3 wrong value */
	int err;
};

static void *child(void *arg)
{
	atomic_fetch_add(&ran, 1);
	return arg;
}

static void *creator_main(void *p)
{
	struct creator *c = p;
	int i, r;

	(void)pthread_barrier_wait(&gate);
	for (i = 0; i < ROUNDS; i++) {
		pthread_t t;
		void *out = NULL;
		uintptr_t v = (uintptr_t)(c->id * 1000 + i + 1);

		r = pthread_create(&t, &attr, child, (void *)v);
		if (r != 0) {
			c->bad = 1;
			c->err = r;
			return NULL;
		}
		r = pthread_join(t, &out);
		if (r != 0) {
			c->bad = 2;
			c->err = r;
			return NULL;
		}
		if ((uintptr_t)out != v) {
			c->bad = 3;
			return NULL;
		}
	}
	return NULL;
}

int main(void)
{
	struct creator c[CREATORS];
	int i, r;

	pc_begin("XSH.pthread_attr_init.003");
	NEED(pthread_attr_init(&attr) == 0, "pthread_attr_init failed");
	NEED(pthread_barrier_init(&gate, NULL, CREATORS + 1) == 0, "pthread_barrier_init failed");
	for (i = 0; i < CREATORS; i++) {
		c[i].id = i + 1;
		c[i].bad = 0;
		c[i].err = 0;
		r = pthread_create(&c[i].thread, NULL, creator_main, &c[i]);
		NEED(r == 0, "pthread_create of a creator thread failed: %s", pc_errname(r));
	}
	pc_test();
	(void)pthread_barrier_wait(&gate);
	for (i = 0; i < CREATORS; i++)
		NEED(pthread_join(c[i].thread, NULL) == 0, "pthread_join of a creator thread failed");
	for (i = 0; i < CREATORS; i++) {
		if (c[i].bad == 1 && c[i].err == EAGAIN)
			pc_unresolved("pthread_create() failed with EAGAIN (no resources)");
		pc_check(c[i].bad != 1, "pthread_create() with the shared attributes object returned %s", pc_errname(c[i].err));
		pc_check(c[i].bad != 2, "pthread_join() returned %s", pc_errname(c[i].err));
		pc_check(c[i].bad != 3, "a new thread did not hand back its argument");
	}
	pc_check(atomic_load(&ran) == CREATORS * ROUNDS, "%d start routines ran, expected %d", atomic_load(&ran),
	    CREATORS * ROUNDS);
	(void)pthread_attr_destroy(&attr);
	pc_pass();
}
