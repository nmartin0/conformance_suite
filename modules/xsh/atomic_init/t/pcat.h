/*
 * pcat.h - shared helper of the <stdatomic.h> modules (the copies in every
 * module's t/ directory are identical).
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * A test defines PCAT_ID (its assertion) and includes this header. When the
 * implementation defines __STDC_NO_ATOMICS__ the header supplies a main()
 * that reports NOTINUSE and sets PCAT_NONE; the test body is then left out
 * with #ifndef PCAT_NONE.
 */
#ifndef PCAT_H
#define PCAT_H

#include "pc.h"

#ifdef __STDC_NO_ATOMICS__

#define PCAT_NONE 1
int main(void)
{
	pc_begin(PCAT_ID);
	pc_test();
	pc_notinuse("__STDC_NO_ATOMICS__ is defined");
}

#else

#include <limits.h>
#include <pthread.h>
#include <sched.h>
#include <stdatomic.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <uchar.h>
#include <unistd.h>
#include <wchar.h>

/* X(tag, direct type, minimum, maximum) for every atomic integer type but bool. */
#define PCAT_INTS(X) \
	X(char, char, CHAR_MIN, CHAR_MAX) \
	X(schar, signed char, SCHAR_MIN, SCHAR_MAX) \
	X(uchar, unsigned char, 0, UCHAR_MAX) \
	X(short, short, SHRT_MIN, SHRT_MAX) \
	X(ushort, unsigned short, 0, USHRT_MAX) \
	X(int, int, INT_MIN, INT_MAX) \
	X(uint, unsigned int, 0, UINT_MAX) \
	X(long, long, LONG_MIN, LONG_MAX) \
	X(ulong, unsigned long, 0, ULONG_MAX) \
	X(llong, long long, LLONG_MIN, LLONG_MAX) \
	X(ullong, unsigned long long, 0, ULLONG_MAX) \
	X(char16_t, char16_t, 0, UINT_LEAST16_MAX) \
	X(char32_t, char32_t, 0, UINT_LEAST32_MAX) \
	X(wchar_t, wchar_t, WCHAR_MIN, WCHAR_MAX) \
	X(int_least8_t, int_least8_t, INT_LEAST8_MIN, INT_LEAST8_MAX) \
	X(uint_least8_t, uint_least8_t, 0, UINT_LEAST8_MAX) \
	X(int_least16_t, int_least16_t, INT_LEAST16_MIN, INT_LEAST16_MAX) \
	X(uint_least16_t, uint_least16_t, 0, UINT_LEAST16_MAX) \
	X(int_least32_t, int_least32_t, INT_LEAST32_MIN, INT_LEAST32_MAX) \
	X(uint_least32_t, uint_least32_t, 0, UINT_LEAST32_MAX) \
	X(int_least64_t, int_least64_t, INT_LEAST64_MIN, INT_LEAST64_MAX) \
	X(uint_least64_t, uint_least64_t, 0, UINT_LEAST64_MAX) \
	X(int_fast8_t, int_fast8_t, INT_FAST8_MIN, INT_FAST8_MAX) \
	X(uint_fast8_t, uint_fast8_t, 0, UINT_FAST8_MAX) \
	X(int_fast16_t, int_fast16_t, INT_FAST16_MIN, INT_FAST16_MAX) \
	X(uint_fast16_t, uint_fast16_t, 0, UINT_FAST16_MAX) \
	X(int_fast32_t, int_fast32_t, INT_FAST32_MIN, INT_FAST32_MAX) \
	X(uint_fast32_t, uint_fast32_t, 0, UINT_FAST32_MAX) \
	X(int_fast64_t, int_fast64_t, INT_FAST64_MIN, INT_FAST64_MAX) \
	X(uint_fast64_t, uint_fast64_t, 0, UINT_FAST64_MAX) \
	X(intptr_t, intptr_t, INTPTR_MIN, INTPTR_MAX) \
	X(uintptr_t, uintptr_t, 0, UINTPTR_MAX) \
	X(size_t, size_t, 0, SIZE_MAX) \
	X(ptrdiff_t, ptrdiff_t, PTRDIFF_MIN, PTRDIFF_MAX) \
	X(intmax_t, intmax_t, INTMAX_MIN, INTMAX_MAX) \
	X(uintmax_t, uintmax_t, 0, UINTMAX_MAX)

/* The same with atomic_bool first. */
#define PCAT_ALL(X) \
	X(bool, _Bool, 0, 1) \
	PCAT_INTS(X)

/* Message form of a value. */
#define PCAT_V(v) ((unsigned long long)(v))

#define PCAT_MAXTH 8

static atomic_int pcat_go;
static atomic_int pcat_abort;

static inline long long pcat_ms(void)
{
	struct timespec ts;

	clock_gettime(CLOCK_MONOTONIC, &ts);
	return (long long)ts.tv_sec * 1000 + ts.tv_nsec / 1000000;
}

/* Called in a spin loop: gives the processor away now and then. */
static inline void pcat_relax(unsigned *n)
{
	if ((++*n & 63u) == 0)
		sched_yield();
}

/* 1 when more than lim ms have passed since t0 (or the run was abandoned). */
static inline int pcat_expired(long long t0, long long lim)
{
	return atomic_load(&pcat_abort) || pcat_ms() - t0 > lim;
}

/* Thread start gate: waits (bounded) until the main thread opens it. */
static inline void pcat_wait_go(void)
{
	unsigned n = 0;
	long long t0 = pcat_ms();

	while (!atomic_load(&pcat_go) && !pcat_expired(t0, 20000))
		pcat_relax(&n);
}

/*
 * Runs fn(index) in n threads (the index passed as a uintptr_t cast to
 * void *), opens the start gate once all exist, and joins them all.
 * Returns -1 when a thread could not be created (the threads made are
 * still joined and told to give up through pcat_abort).
 */
static inline int pcat_par(int n, void *(*fn)(void *))
{
	pthread_t th[PCAT_MAXTH];
	int i, made = 0, rc = 0;

	alarm(150);
	atomic_store(&pcat_go, 0);
	atomic_store(&pcat_abort, 0);
	for (i = 0; i < n; i++) {
		if (pthread_create(&th[i], NULL, fn, (void *)(uintptr_t)i) != 0) {
			atomic_store(&pcat_abort, 1);
			rc = -1;
			break;
		}
		made++;
	}
	atomic_store(&pcat_go, 1);
	for (i = 0; i < made; i++)
		pthread_join(th[i], NULL);
	return rc;
}

/* The six memory orders. */
static const memory_order pcat_orders[6] = {
	memory_order_relaxed, memory_order_consume, memory_order_acquire,
	memory_order_release, memory_order_acq_rel, memory_order_seq_cst
};

static inline const char *pcat_oname(memory_order o)
{
	switch (o) {
	case memory_order_relaxed: return "relaxed";
	case memory_order_consume: return "consume";
	case memory_order_acquire: return "acquire";
	case memory_order_release: return "release";
	case memory_order_acq_rel: return "acq_rel";
	case memory_order_seq_cst: return "seq_cst";
	}
	return "?";
}

#endif /* __STDC_NO_ATOMICS__ */
#endif /* PCAT_H */
