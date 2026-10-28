/*
 * A new thread starts with cancelability enabled and deferred whatever
 * its creator's setting.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.2.9-threads.011
 */
#include "pcth.h"

static void *body(void *a)
{
	int old_type = -1, old_state = -1;

	(void)a;
	flag_set(2, pthread_setcanceltype(PTHREAD_CANCEL_DEFERRED, &old_type));
	flag_set(3, pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, &old_state));
	flag_set(0, old_type == PTHREAD_CANCEL_DEFERRED ? 1 : 2);
	flag_set(1, old_state == PTHREAD_CANCEL_ENABLE ? 1 : 2);
	return NULL;
}

int main(void)
{
	pthread_t t;
	int old;

	pc_begin("XSH.2.9-threads.011");
	guard(60);
	pc_test();
	NEED(pthread_setcancelstate(PTHREAD_CANCEL_DISABLE, &old) == 0, "pthread_setcancelstate failed");
	NEED(pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, &old) == 0, "pthread_setcanceltype failed");
	t = start(body, NULL);
	NEED(pthread_join(t, NULL) == 0, "pthread_join failed");
	(void)pthread_setcanceltype(PTHREAD_CANCEL_DEFERRED, &old);
	(void)pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, &old);
	NEED(flag_get(2) == 0 && flag_get(3) == 0, "pthread_setcanceltype() or pthread_setcancelstate() failed in the new thread");
	pc_check(flag_get(0) == 1, "the new thread's cancelability type was not PTHREAD_CANCEL_DEFERRED although its creator had set ASYNCHRONOUS");
	pc_check(flag_get(1) == 1, "the new thread's cancelability state was not PTHREAD_CANCEL_ENABLE although its creator had set DISABLE");
	pc_pass();
}
