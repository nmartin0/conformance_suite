/*
 * pthread_getspecific.c - planted faults in pthread_getspecific() and
 * pthread_setspecific(), for mutation checks (CI only).
 *
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * A preload library; faults are named "pthread_getspecific.<fault>" and
 * apply only while PC_PHASE is "test". Keys made with a destructor get a
 * trampoline that marks the thread as being in a destructor.
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

#define PREFIX "pthread_getspecific."

#define NK 32

static pthread_key_t keys[NK];
static void (*dtors[NK])(void *);
static void *shared_vals[NK];
static int have_shared[NK];
static void *first_vals[NK];
static int have_first[NK];
static int nkeys;
static pthread_mutex_t lk = PTHREAD_MUTEX_INITIALIZER;
static __thread pthread_key_t in_dtor_key;
static __thread int in_dtor;
static __thread void *dtor_value;

static int index_of(pthread_key_t k)
{
	int i;

	for (i = 0; i < nkeys; i++)
		if (keys[i] == k)
			return i;
	return -1;
}

static void tramp(int i, void *v)
{
	in_dtor = 1;
	in_dtor_key = keys[i];
	dtor_value = v;
	dtors[i](v);
	in_dtor = 0;
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
	int i, r;

	(void)pthread_mutex_lock(&lk);
	i = nkeys < NK ? nkeys++ : -1;
	(void)pthread_mutex_unlock(&lk);
	r = real_pthread_key_create(key, i >= 0 && dtor != NULL ? tramps[i] : dtor);
	if (r == 0 && i >= 0) {
		keys[i] = *key;
		dtors[i] = dtor;
	}
	return r;
}

int pthread_setspecific(pthread_key_t k, const void *v)
{
	REAL(pthread_setspecific);
	const char *m = mutant_of(PREFIX);
	int i = index_of(k);

	if (IS("set_ignored"))
		return 0;
	if (IS("set_null_fails") && v == NULL)
		return ENOMEM;
	if (IS("dtor_set_lost") && in_dtor && k == in_dtor_key)
		return 0;	/* a value set in the key's own destructor is dropped */
	if (IS("shared") && i >= 0) {
		(void)pthread_mutex_lock(&lk);
		shared_vals[i] = (void *)v;	/* one value for all threads */
		have_shared[i] = 1;
		(void)pthread_mutex_unlock(&lk);
	}
	if (IS("first_value_kept") && i >= 0) {
		if (!have_first[i]) {
			first_vals[i] = (void *)v;
			have_first[i] = 1;
		}
		v = first_vals[i];
	}
	return real_pthread_setspecific(k, v);
}

void *pthread_getspecific(pthread_key_t k)
{
	REAL(pthread_getspecific);
	const char *m = mutant_of(PREFIX);
	void *v = real_pthread_getspecific(k);
	int i = index_of(k);

	if (IS("get_null"))
		return NULL;
	if (IS("shared") && i >= 0 && have_shared[i]) {
		(void)pthread_mutex_lock(&lk);
		v = shared_vals[i];
		(void)pthread_mutex_unlock(&lk);
	}
	if (IS("dtor_sees_value") && in_dtor && k == in_dtor_key && v == NULL)
		return dtor_value;	/* the value is not cleared before the call */
	return v;
}
