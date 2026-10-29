/*
 * Every thread's copy of an uninitialised thread-local object starts as zero.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION ABI.i386.threads.009
 */
#include "../../../../check.h"
#include <pthread.h>
#include <string.h>

#ifndef ABI_ARCH_I386
ABI_NOT_THIS_ARCH("ABI.i386.threads.009")
#else
static _Thread_local unsigned char tl_buf[20000];
static _Thread_local long tl_word;
static _Thread_local struct { int a[200]; } tl_big;

static void *tbody(void *arg)
{
	long *bad = arg;
	size_t i;

	*bad = 0;
	for (i = 0; i < sizeof tl_buf; i++)
		if (tl_buf[i] != 0)
			(*bad)++;
	if (tl_word != 0)
		(*bad)++;
	for (i = 0; i < 200; i++)
		if (tl_big.a[i] != 0)
			(*bad)++;
	return NULL;
}
int main(void)
{
	pc_begin("ABI.i386.threads.009");
	pthread_t t;
	long nonzero = -1;
	size_t i;
	int k;

	pc_test();
	for (i = 0; i < sizeof tl_buf; i++)
		pc_check(tl_buf[i] == 0, "the main thread's tl_buf[%lu] is not zero at the start", (unsigned long)i);
	memset(tl_buf, 0xFF, sizeof tl_buf);    /* dirty the main thread's copy */
	tl_word = -1;
	tl_big.a[199] = 5;
	for (k = 0; k < 3; k++) {
		nonzero = -1;
		if (pthread_create(&t, NULL, tbody, &nonzero) != 0)
			pc_unresolved("pthread_create failed");
		pthread_join(t, NULL);
		pc_check(nonzero == 0, "thread %d: %ld thread-local byte(s) or word(s) were not zero at its start", k, nonzero);
	}
	pc_pass();
}
#endif
