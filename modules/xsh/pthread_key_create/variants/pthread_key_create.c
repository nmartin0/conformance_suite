/*
 * pthread_key_create.c - conforming variants of thread-specific data
 * keys, for the variant checks (CI only).
 *
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * A preload library; see core/pcvariant.h. Each variant says why the
 * standard allows it.
 *
 * "dtors_reversed": at thread exit the destructors run for the most
 * recently created key first. XSH pthread_key_create() leaves the order
 * of destructor calls unspecified. A key of the variant's own, made
 * before any other, is given a value in every thread; the C library runs
 * its destructor first, and that destructor runs the other keys'
 * destructors in reverse order of creation, repeating (up to the four
 * rounds {_POSIX_THREAD_DESTRUCTOR_ITERATIONS} guarantees) while values
 * are set again, so the library then finds them all NULL. It runs after
 * the cleanup handlers, as XSH 2.9.5.3 requires.
 *
 * "keys_max_128": {PTHREAD_KEYS_MAX} is 128, the least XBD <limits.h>
 * allows ({_POSIX_THREAD_KEYS_MAX}), reported by sysconf(); a creation
 * beyond 128 live keys fails with EAGAIN, as ERRORS requires.
 *
 * "keys_enomem": running out of keys is reported as ENOMEM. Both EAGAIN
 * and ENOMEM are shall-fail errors of pthread_key_create(), and when the
 * key table is full both conditions hold, so XSH 2.3 lets either be
 * returned.
 */
#define _GNU_SOURCE 1
#include <errno.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include "pcvariant.h"

#define NK 64

static pthread_mutex_t lk = PTHREAD_MUTEX_INITIALIZER;
static pthread_key_t keys[NK];
static void (*dtors[NK])(void *);
static int alive[NK], nkeys, live_keys;
static pthread_key_t master;
static int have_master;

static void run_reversed(void *p)
{
	int round, i, again = 1;

	(void)p;
	for (round = 0; round < 4 && again; round++) {
		again = 0;
		for (i = nkeys - 1; i >= 0; i--) {
			void *v;
			if (!alive[i] || dtors[i] == NULL || (v = pthread_getspecific(keys[i])) == NULL)
				continue;
			(void)pthread_setspecific(keys[i], NULL);
			dtors[i](v);
			again = 1;
		}
		for (i = 0; i < nkeys && again; i++)
			if (alive[i] && dtors[i] != NULL && pthread_getspecific(keys[i]) != NULL)
				break;
		if (i == nkeys)
			again = 0;
	}
}

__attribute__((constructor)) static void make_master(void)
{
	const char *v = variant_of("pthread_key_create.");
	REAL(pthread_key_create);

	if (IS("dtors_reversed") && real_pthread_key_create(&master, run_reversed) == 0) {
		have_master = 1;
		(void)pthread_setspecific(master, (void *)1);
	}
}

long sysconf(int name)
{
	const char *v = variant_of("pthread_key_create.");
	REAL(sysconf);

	if (IS("keys_max_128") && name == _SC_THREAD_KEYS_MAX)
		return 128;
	return real_sysconf(name);
}

int pthread_key_create(pthread_key_t *key, void (*dtor)(void *))
{
	const char *v = variant_of("pthread_key_create.");
	REAL(pthread_key_create);
	int r;

	if (IS("keys_max_128")) {
		(void)pthread_mutex_lock(&lk);
		if (live_keys >= 128) {
			(void)pthread_mutex_unlock(&lk);
			return EAGAIN;
		}
		live_keys++;
		(void)pthread_mutex_unlock(&lk);
	}
	if (IS("dtors_reversed") && have_master) {
		(void)pthread_mutex_lock(&lk);
		r = real_pthread_key_create(key, NULL);	/* the master key runs dtor */
		if (r == 0 && nkeys < NK) {
			keys[nkeys] = *key;
			dtors[nkeys] = dtor;
			alive[nkeys++] = 1;
		} else if (r == 0) {
			int (*del)(pthread_key_t) = (int (*)(pthread_key_t))dlsym(RTLD_NEXT, "pthread_key_delete");
			(void)del(*key);
			r = EAGAIN;
		}
		(void)pthread_mutex_unlock(&lk);
		return r;
	}
	r = real_pthread_key_create(key, dtor);
	if (r != 0 && IS("keys_max_128")) {
		(void)pthread_mutex_lock(&lk);
		live_keys--;
		(void)pthread_mutex_unlock(&lk);
	}
	if (r == EAGAIN && IS("keys_enomem"))
		return ENOMEM;
	return r;
}

int pthread_key_delete(pthread_key_t key)
{
	const char *v = variant_of("pthread_key_create.");
	REAL(pthread_key_delete);
	int i, r = real_pthread_key_delete(key);

	if (r == 0 && IS("keys_max_128")) {
		(void)pthread_mutex_lock(&lk);
		live_keys--;
		(void)pthread_mutex_unlock(&lk);
	}
	if (r == 0 && IS("dtors_reversed")) {
		(void)pthread_mutex_lock(&lk);
		for (i = 0; i < nkeys; i++)
			if (alive[i] && keys[i] == key)
				alive[i] = 0;
		(void)pthread_mutex_unlock(&lk);
	}
	return r;
}

struct start {
	void *(*fn)(void *);
	void *arg;
};

static void *trampoline(void *p)
{
	struct start w = *(struct start *)p;

	free(p);
	(void)pthread_setspecific(master, (void *)1);
	return w.fn(w.arg);
}

int pthread_create(pthread_t *restrict t, const pthread_attr_t *restrict attr,
		   void *(*fn)(void *), void *restrict arg)
{
	const char *v = variant_of("pthread_key_create.");
	struct start *w;
	int r;
	REAL(pthread_create);

	if (!IS("dtors_reversed") || !have_master || (w = malloc(sizeof *w)) == NULL)
		return real_pthread_create(t, attr, fn, arg);
	w->fn = fn;
	w->arg = arg;
	r = real_pthread_create(t, attr, trampoline, w);
	if (r != 0)
		free(w);
	return r;
}
