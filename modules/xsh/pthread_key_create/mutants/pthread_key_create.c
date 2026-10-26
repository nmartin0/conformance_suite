/*
 * pthread_key_create.c - planted faults in pthread_key_create() and the
 * destructor calls it sets up, for mutation checks (CI only).
 *
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * A preload library; faults are named "pthread_key_create.<fault>" and
 * apply only while PC_PHASE is "test". Keys made while a fault is active
 * get a destructor trampoline, so the faults can change how destructors
 * are called.
 */
#define _GNU_SOURCE 1
#include <dlfcn.h>
#include <errno.h>
#include <pthread.h>
#include <signal.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

static char phase[16];

/* pc.c sets PC_PHASE with setenv(); remember it in this process. */
int setenv(const char *name, const char *value, int overwrite)
{
	static int (*real)(const char *, const char *, int);

	if (real == NULL)
		real = (int (*)(const char *, const char *, int))dlsym(RTLD_NEXT, "setenv");
	if (strcmp(name, "PC_PHASE") == 0 && strlen(value) < sizeof phase)
		strcpy(phase, value);
	return real(name, value, overwrite);
}

static const char *mutant_of(const char *prefix)
{
	const char *m = getenv("PC_MUTANT");
	size_t n = strlen(prefix);

	if (strcmp(phase, "test") != 0 || m == NULL || strncmp(m, prefix, n) != 0)
		return NULL;
	return m + n;
}

#define IS(name) (m != NULL && strcmp(m, name) == 0)
#define REAL(fn) static __typeof__(fn) *real_##fn; \
	if (real_##fn == NULL) real_##fn = (__typeof__(fn) *)dlsym(RTLD_NEXT, #fn)

static void __attribute__((unused)) nap_ms(long ms)
{
	struct timespec ts = { ms / 1000, (ms % 1000) * 1000000L };

	while (nanosleep(&ts, &ts) == -1 && errno == EINTR)
		;
}

#define PREFIX "pthread_key_create."

#define NK 32

static pthread_key_t keys[NK];
static void (*dtors[NK])(void *);
static pthread_t creators[NK];
static int stale_from[NK];	/* index of a deleted key whose values leak into this one, or -1 */
static int nkeys, last_deleted = -1;
static pthread_mutex_t lk = PTHREAD_MUTEX_INITIALIZER;
static int dummy;
static __thread void *set_vals[NK];	/* last value this thread set per tracked key */

static int index_of(pthread_key_t k)
{
	int i;

	for (i = nkeys - 1; i >= 0; i--)	/* the newest key of that value */
		if (keys[i] == k)
			return i;
	return -1;
}

static void tramp(int i, void *v)
{
	const char *m = mutant_of(PREFIX);
	void (*d)(void *) = dtors[i];

	if (d == NULL || IS("no_dtor"))
		return;
	d(IS("dtor_arg_null") ? NULL : v);
	if (IS("single_pass"))
		(void)pthread_setspecific(keys[i], NULL);
	if (IS("extra_pass") && pthread_getspecific(keys[i]) == NULL)
		d(NULL);	/* one call too many, for a NULL value */
}

#define T(n) static void tramp##n(void *v) { tramp(n, v); }
T(0) T(1) T(2) T(3) T(4) T(5) T(6) T(7) T(8) T(9) T(10) T(11) T(12) T(13) T(14) T(15)
T(16) T(17) T(18) T(19) T(20) T(21) T(22) T(23) T(24) T(25) T(26) T(27) T(28) T(29) T(30) T(31)
static void (*const tramps[NK])(void *) = {
	tramp0, tramp1, tramp2, tramp3, tramp4, tramp5, tramp6, tramp7, tramp8, tramp9, tramp10,
	tramp11, tramp12, tramp13, tramp14, tramp15, tramp16, tramp17, tramp18, tramp19, tramp20,
	tramp21, tramp22, tramp23, tramp24, tramp25, tramp26, tramp27, tramp28, tramp29, tramp30, tramp31
};

int pthread_key_create(pthread_key_t *key, void (*dtor)(void *))
{
	REAL(pthread_key_create);
	const char *m = mutant_of(PREFIX);
	int i, r;

	/* Every key is followed, so that faults reach keys made during setup. */
	(void)pthread_mutex_lock(&lk);
	i = nkeys < NK ? nkeys : -1;
	if (i >= 0)
		nkeys++;
	(void)pthread_mutex_unlock(&lk);
	r = real_pthread_key_create(key, i >= 0 && dtor != NULL ? tramps[i] : dtor);
	if (r == EAGAIN && IS("limit_einval"))
		return EINVAL;
	if (r != 0 || i < 0)
		return r;
	keys[i] = *key;
	dtors[i] = dtor;
	creators[i] = pthread_self();
	stale_from[i] = IS("stale_after_delete") ? last_deleted : -1;
	if (IS("creator_value"))
		(void)pthread_setspecific(*key, &dummy);
	return 0;
}

int pthread_key_delete(pthread_key_t k)
{
	REAL(pthread_key_delete);
	int i = index_of(k);

	if (i >= 0)
		last_deleted = i;
	return real_pthread_key_delete(k);
}

int pthread_setspecific(pthread_key_t k, const void *v)
{
	REAL(pthread_setspecific);
	int i = index_of(k);

	if (i >= 0)
		set_vals[i] = (void *)v;
	return real_pthread_setspecific(k, v);
}

void *pthread_getspecific(pthread_key_t k)
{
	REAL(pthread_getspecific);
	const char *m = mutant_of(PREFIX);
	void *v = real_pthread_getspecific(k);
	int i;

	if (m == NULL || v != NULL || (i = index_of(k)) < 0)
		return v;
	if (IS("others_value") && !pthread_equal(creators[i], pthread_self()))
		return &dummy;
	if (IS("stale_after_delete") && stale_from[i] >= 0)
		return set_vals[stale_from[i]];
	return v;
}

struct start {
	void *(*fn)(void *);
	void *arg;
	void *vals[NK];
	int n;
};

static void *trampoline(void *p)
{
	struct start w = *(struct start *)p;
	const char *m = mutant_of(PREFIX);
	void *r;
	int i;

	free(p);
	if (IS("inherit_values"))
		for (i = 0; i < w.n; i++)
			if (w.vals[i] != NULL)
				(void)pthread_setspecific(keys[i], w.vals[i]);
	r = w.fn(w.arg);
	if (IS("dtor_on_null"))
		for (i = 0; i < nkeys; i++)
			if (dtors[i] != NULL && pthread_getspecific(keys[i]) == NULL)
				dtors[i](NULL);
	return r;
}

int pthread_create(pthread_t *restrict t, const pthread_attr_t *restrict attr,
		   void *(*fn)(void *), void *restrict arg)
{
	REAL(pthread_create);
	struct start *w;
	int i, r;

	if (mutant_of(PREFIX) == NULL || (w = malloc(sizeof *w)) == NULL)
		return real_pthread_create(t, attr, fn, arg);
	w->fn = fn;
	w->arg = arg;
	w->n = nkeys;
	for (i = 0; i < w->n; i++)
		w->vals[i] = pthread_getspecific(keys[i]);
	r = real_pthread_create(t, attr, trampoline, w);
	if (r != 0)
		free(w);
	return r;
}
