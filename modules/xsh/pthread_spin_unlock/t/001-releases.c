/*
 * pthread_spin_unlock() frees a lock taken with lock or trylock.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.pthread_spin_unlock.001
 */
#include "pc.h"
#include <errno.h>
#include <pthread.h>

#define NEED(cond, ...) do { if (!(cond)) pc_unresolved(__VA_ARGS__); } while (0)

static const char *rn(int r)
{
	return r == 0 ? "0" : pc_errname(r);
}

static pthread_spinlock_t sp;

static void *other_try(void *arg)
{
	int r = pthread_spin_trylock(&sp);

	(void)arg;
	if (r == 0)
		(void)pthread_spin_unlock(&sp);
	return (void *)(long)r;
}

static int in_thread(void)
{
	pthread_t t;
	void *out;

	NEED(pthread_create(&t, NULL, other_try, NULL) == 0, "pthread_create failed");
	NEED(pthread_join(t, &out) == 0, "pthread_join failed");
	return (int)(long)out;
}

int main(void)
{
	int r, i;

	pc_begin("XSH.pthread_spin_unlock.001");
	NEED(pthread_spin_init(&sp, PTHREAD_PROCESS_PRIVATE) == 0, "pthread_spin_init failed");
	pc_test();
	for (i = 0; i < 2; i++) {
		const char *how = i == 0 ? "pthread_spin_lock" : "pthread_spin_trylock";

		r = i == 0 ? pthread_spin_lock(&sp) : pthread_spin_trylock(&sp);
		NEED(r == 0, "%s failed: %s", how, rn(r));
		r = in_thread();
		NEED(r == EBUSY, "after %s, another thread's trylock returned %s, expected EBUSY", how, rn(r));
		r = pthread_spin_unlock(&sp);
		pc_check(r == 0, "pthread_spin_unlock() of a lock taken with %s returned %s", how, rn(r));
		r = in_thread();
		pc_check(r == 0, "after unlocking a lock taken with %s, another thread's trylock returned %s, expected 0", how,
		    rn(r));
	}
	pc_pass();
}
