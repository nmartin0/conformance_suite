/*
 * PTHREAD_CANCELED is a constant expression of type void * that is not
 * null and equals no pointer to an object, and a canceled thread's
 * status is that value.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.2.9-threads.027
 */
#include "pcth.h"

/* Only a constant expression may initialize an object with static storage duration. */
static void *const canceled = PTHREAD_CANCELED;
static int gobj;
static char garr[16];
static int pfd[2];

static void *reader(void *a)
{
	char b;

	(void)a;
	flag_set(0, 1);
	if (read(pfd[0], &b, 1) < 0)
		flag_set(1, 1);
	return NULL;
}

static void *returns_null(void *a)
{
	(void)a;
	return NULL;
}

int main(void)
{
	static int sobj;
	int lobj;
	void *heap, *st;
	pthread_t t;

	pc_begin("XSH.2.9-threads.027");
	guard(60);
	heap = malloc(32);
	NEED(heap != NULL, "malloc failed");
	NEED(pipe(pfd) == 0, "pipe failed");
	pc_test();
	pc_check(_Generic(PTHREAD_CANCELED, void *: 1, default: 0), "PTHREAD_CANCELED does not have type void *");
	pc_check(canceled != NULL, "PTHREAD_CANCELED is a null pointer");
	pc_check(canceled != (void *)&gobj && canceled != (void *)garr && canceled != (void *)&garr[15] && canceled != (void *)&sobj &&
	    canceled != (void *)&lobj && canceled != heap && canceled != (void *)&canceled && canceled != (void *)stdout &&
	    canceled != (void *)stderr, "PTHREAD_CANCELED equals the address of an object");
	t = start(returns_null, NULL);
	NEED(join_within(t, 10000, &st), "a thread did not end");
	pc_check(st == NULL && st != canceled, "a thread that returned a null pointer was joined with a different value");
	t = start(reader, NULL);
	NEED(wait_flag(0, 1, 10000), "the reader did not start");
	sleep_ms(100);
	NEED(pthread_cancel(t) == 0, "pthread_cancel failed");
	NEED(join_within(t, 10000, &st), "the canceled thread did not end");
	pc_check(st == canceled, "a canceled thread was joined with a status other than PTHREAD_CANCELED");
	pc_check(flag_get(1) == 0, "read() returned in the canceled thread");
	free(heap);
	pc_pass();
}
