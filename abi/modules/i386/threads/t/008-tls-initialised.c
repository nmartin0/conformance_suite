/*
 * Every thread's copy of an initialised thread-local object starts with the initial value.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION ABI.i386.threads.008
 */
#include "../../../../check.h"
#include <pthread.h>
#include <string.h>

#ifndef ABI_ARCH_I386
ABI_NOT_THIS_ARCH("ABI.i386.threads.008")
#else
static _Thread_local int tl_int = 0x1234ABCD;
static _Thread_local char tl_text[8] = "hello";
static _Thread_local int tl_arr[5] = { 1, 2, 3, 4, 5 };

struct report { int i; char text[8]; int arr3; };

static void *tbody(void *arg)
{
	struct report *r = arg;

	r->i = tl_int;
	memcpy(r->text, tl_text, sizeof r->text);
	r->arr3 = tl_arr[3];
	tl_int = 77;                    /* changing it must stay private */
	strcpy(tl_text, "ZZZZZ");
	return NULL;
}
int main(void)
{
	pc_begin("ABI.i386.threads.008");
	pthread_t t;
	struct report r;
	int i;

	pc_test();
	pc_check(tl_int == 0x1234ABCD && strcmp(tl_text, "hello") == 0 && tl_arr[3] == 4,
	    "the main thread's initialised thread-local data is wrong (0x%x, \"%s\", %d)", tl_int, tl_text, tl_arr[3]);
	tl_int = 1;                     /* the main thread changes its copy; new threads must not see it */
	strcpy(tl_text, "XXXXX");
	tl_arr[3] = 99;
	for (i = 0; i < 3; i++) {
		memset(&r, 0, sizeof r);
		if (pthread_create(&t, NULL, tbody, &r) != 0)
			pc_unresolved("pthread_create failed");
		pthread_join(t, NULL);
		pc_check(r.i == 0x1234ABCD, "thread %d saw tl_int = 0x%x, expected the initial 0x1234abcd", i, r.i);
		pc_check(strcmp(r.text, "hello") == 0, "thread %d saw tl_text = \"%s\", expected \"hello\"", i, r.text);
		pc_check(r.arr3 == 4, "thread %d saw tl_arr[3] = %d, expected 4", i, r.arr3);
		pc_check(tl_int == 1, "the main thread's tl_int became %d after thread %d ran", tl_int, i);
	}
	pc_pass();
}
#endif
