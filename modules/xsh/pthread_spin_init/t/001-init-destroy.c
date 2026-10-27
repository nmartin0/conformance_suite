/*
 * pthread_spin_init() gives an unlocked lock; destroy and init can be repeated.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.pthread_spin_init.001
 */
#include "pc.h"
#include <errno.h>
#include <pthread.h>


static const char *rn(int r)
{
	return r == 0 ? "0" : pc_errname(r);
}

static void check_free(pthread_spinlock_t *s, const char *when)
{
	int r;

	r = pthread_spin_trylock(s);
	pc_check(r == 0, "%s: pthread_spin_trylock() returned %s", when, rn(r));
	r = pthread_spin_trylock(s);
	pc_check(r == EBUSY, "%s: pthread_spin_trylock() of the held lock returned %s, expected EBUSY", when, rn(r));
	r = pthread_spin_unlock(s);
	pc_check(r == 0, "%s: pthread_spin_unlock() returned %s", when, rn(r));
}

int main(void)
{
	pthread_spinlock_t s;
	int r;

	pc_begin("XSH.pthread_spin_init.001");
	pc_test();
	r = pthread_spin_init(&s, PTHREAD_PROCESS_PRIVATE);
	pc_check(r == 0, "pthread_spin_init() returned %s", rn(r));
	check_free(&s, "after init");
	r = pthread_spin_destroy(&s);
	pc_check(r == 0, "pthread_spin_destroy() returned %s", rn(r));
	r = pthread_spin_init(&s, PTHREAD_PROCESS_PRIVATE);
	pc_check(r == 0, "pthread_spin_init() after the destroy returned %s", rn(r));
	check_free(&s, "after destroy and init");
	r = pthread_spin_destroy(&s);
	pc_check(r == 0, "second pthread_spin_destroy() returned %s", rn(r));
	pc_pass();
}
