/*
 * pthread_getspecific() returns what pthread_setspecific() last set.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.pthread_getspecific.001
 * MUTANT    pthread_getspecific.get_null
 * MUTANT    pthread_getspecific.set_ignored
 * MUTANT    pthread_getspecific.set_null_fails
 * MUTANT    pthread_getspecific.first_value_kept
 */
#include "pc.h"
#include <errno.h>
#include <pthread.h>

static int a_obj, b_obj;

int main(void)
{
	pthread_key_t key;
	void *g0, *g1, *g2, *g3;
	int r1, r2, r3, r;

	pc_begin("XSH.pthread_getspecific.001");
	r = pthread_key_create(&key, NULL);
	pc_check(r == 0, "pthread_key_create failed: %s", pc_errname(r));
	pc_test();
	g0 = pthread_getspecific(key);
	r1 = pthread_setspecific(key, &a_obj);
	g1 = pthread_getspecific(key);
	r2 = pthread_setspecific(key, &b_obj);
	g2 = pthread_getspecific(key);
	r3 = pthread_setspecific(key, NULL);
	g3 = pthread_getspecific(key);
	pc_check(g0 == NULL, "before any value was set, pthread_getspecific returned %p", g0);
	pc_check(r1 == 0 && r2 == 0, "pthread_setspecific returned %s and %s, expected 0", pc_errname(r1), pc_errname(r2));
	pc_check(r3 == 0, "pthread_setspecific(key, NULL) returned %s, expected 0", pc_errname(r3));
	pc_check(g1 == &a_obj, "after setting %p, pthread_getspecific returned %p", (void *)&a_obj, g1);
	pc_check(g2 == &b_obj, "after setting %p, pthread_getspecific returned %p", (void *)&b_obj, g2);
	pc_check(g3 == NULL, "after setting NULL, pthread_getspecific returned %p", g3);
	pc_pass();
}
