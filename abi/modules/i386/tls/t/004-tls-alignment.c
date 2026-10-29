/*
 * Thread-local objects declared with a strict alignment have it, in the main thread and in new
 * threads.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION ABI.i386.tls.004
 */
#include "../../../../check.h"
#include <pthread.h>
#include <string.h>
#include <stdint.h>

#ifndef ABI_ARCH_I386
ABI_NOT_THIS_ARCH("ABI.i386.tls.004")
#else
static _Thread_local _Alignas(16) int d16 = 1;
static _Thread_local _Alignas(64) int d64 = 2;
static _Thread_local _Alignas(256) char d256[10] = "abc";
static _Thread_local _Alignas(4096) int d4096 = 3;
static _Thread_local _Alignas(16) char z16[3];
static _Thread_local _Alignas(64) int z64;
static _Thread_local _Alignas(256) int z256[3];
static _Thread_local _Alignas(4096) char z4096[100];

static unsigned check_all(void)
{
	unsigned m = 0;

	m |= ((uintptr_t)&d16 % 16) ? 1u : 0;
	m |= ((uintptr_t)&d64 % 64) ? 2u : 0;
	m |= ((uintptr_t)d256 % 256) ? 4u : 0;
	m |= ((uintptr_t)&d4096 % 4096) ? 8u : 0;
	m |= ((uintptr_t)z16 % 16) ? 16u : 0;
	m |= ((uintptr_t)&z64 % 64) ? 32u : 0;
	m |= ((uintptr_t)z256 % 256) ? 64u : 0;
	m |= ((uintptr_t)z4096 % 4096) ? 128u : 0;
	/* and the values must still be there */
	m |= (d16 == 1 && d64 == 2 && strcmp(d256, "abc") == 0 && d4096 == 3) ? 0 : 256u;
	return m;
}

static void *tbody(void *arg)
{
	*(unsigned *)arg = check_all();
	return NULL;
}
int main(void)
{
	pc_begin("ABI.i386.tls.004");
	pthread_t t;
	unsigned bad = 0;
	int k;

	pc_test();
	pc_check(check_all() == 0, "the main thread's thread-local objects are not aligned as declared (mask 0x%x)", check_all());
	for (k = 0; k < 3; k++) {
		if (pthread_create(&t, NULL, tbody, &bad) != 0)
			pc_unresolved("pthread_create failed");
		pthread_join(t, NULL);
		pc_check(bad == 0, "thread %d: thread-local objects are not aligned as declared (mask 0x%x)", k, bad);
	}
	pc_pass();
}
#endif
