/*
 * pthread_join() reports the value given to pthread_exit() or returned.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.pthread_join.003
 * MUTANT    pthread_join.value_null
 * MUTANT    pthread_join.value_untouched_for_null
 * MUTANT    pthread_join.null_vp_einval
 * VARIANT   pthread_create.runs_late
 */
#include "pc.h"
#include <errno.h>
#include <pthread.h>
#include <time.h>

static int a_obj, b_obj;

static void leave(void *v)
{
	pthread_exit(v);
}

static void *by_exit(void *arg)
{
	leave(arg);
	return &b_obj;	/* not reached */
}

static void *by_return(void *arg)
{
	(void)arg;
	return &b_obj;
}

int main(void)
{
	pthread_t t[4];
	void *v[3] = { &b_obj, &b_obj, NULL };
	int r, i;

	pc_begin("XSH.pthread_join.003");
	pc_test();
	r = pthread_create(&t[0], NULL, by_exit, &a_obj);
	if (r == 0)
		r = pthread_create(&t[1], NULL, by_exit, NULL);
	if (r == 0)
		r = pthread_create(&t[2], NULL, by_return, NULL);
	if (r == 0)
		r = pthread_create(&t[3], NULL, by_return, NULL);
	if (r != 0)
		pc_unresolved("pthread_create failed: %s", pc_errname(r));
	for (i = 0; i < 3; i++) {
		r = pthread_join(t[i], &v[i]);
		pc_check(r == 0, "pthread_join of thread %d returned %s, expected 0", i, pc_errname(r));
	}
	r = pthread_join(t[3], NULL);
	pc_check(r == 0, "pthread_join with a null value_ptr returned %s, expected 0", pc_errname(r));
	pc_check(v[0] == &a_obj, "pthread_exit(%p) was joined as %p", (void *)&a_obj, v[0]);
	pc_check(v[1] == NULL, "pthread_exit(NULL) was joined as %p", v[1]);
	pc_check(v[2] == &b_obj, "a start routine returning %p was joined as %p", (void *)&b_obj, v[2]);
	pc_pass();
}
