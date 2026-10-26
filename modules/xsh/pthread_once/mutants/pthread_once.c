/*
 * pthread_once.c - planted faults in pthread_once(), for mutation checks
 * (CI only).
 *
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * A preload library; faults are named "pthread_once.<fault>" and apply
 * only while PC_PHASE is "test". Several replace pthread_once() with a
 * faulty one keyed by the once_control's address.
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

#define PREFIX "pthread_once."

static pthread_mutex_t lk = PTHREAD_MUTEX_INITIALIZER;
static pthread_cond_t cv = PTHREAD_COND_INITIALIZER;
static struct { pthread_once_t *c; int state; } ctl[64];	/* state: 1 running, 2 done */
static int any_called;

static int *state_of(pthread_once_t *c)
{
	int i;

	for (i = 0; i < 64; i++)
		if (ctl[i].c == c)
			return &ctl[i].state;
	for (i = 0; i < 64; i++)
		if (ctl[i].c == NULL) {
			ctl[i].c = c;
			return &ctl[i].state;
		}
	abort();
}

static void mark_done(void *p)
{
	(void)pthread_mutex_lock(&lk);
	*(int *)p = 2;	/* a canceled init_routine counts as done */
	(void)pthread_cond_broadcast(&cv);
	(void)pthread_mutex_unlock(&lk);
}

int pthread_once(pthread_once_t *c, void (*init)(void))
{
	REAL(pthread_once);
	const char *m = mutant_of(PREFIX);
	static int calls;
	int *st, first;

	if (m == NULL)
		return real_pthread_once(c, init);
	if (IS("testcancel"))
		pthread_testcancel();
	if (IS("always_calls")) {
		init();
		return 0;
	}
	if (IS("returns_one")) {
		int r = real_pthread_once(c, init);
		return r == 0 && ++calls > 1 ? 1 : r;
	}
	if (IS("one_control")) {
		(void)pthread_mutex_lock(&lk);
		first = !any_called;
		any_called = 1;
		(void)pthread_mutex_unlock(&lk);
		if (first)
			init();
		return 0;
	}
	if (!(IS("no_wait") || IS("cancel_done") || IS("eintr")))
		return real_pthread_once(c, init);
	(void)pthread_mutex_lock(&lk);
	st = state_of(c);
	first = *st == 0;
	if (first)
		*st = 1;
	(void)pthread_mutex_unlock(&lk);
	if (first) {
		pthread_cleanup_push(mark_done, st);
		init();
		pthread_cleanup_pop(1);
		return 0;
	}
	if (IS("no_wait"))
		return 0;	/* does not wait for the running init_routine */
	if (IS("eintr"))
		for (;;) {
			struct timespec ts = { 0, 5000000L };
			(void)pthread_mutex_lock(&lk);
			first = *st == 2;
			(void)pthread_mutex_unlock(&lk);
			if (first)
				return 0;
			if (nanosleep(&ts, NULL) == -1 && errno == EINTR)
				return EINTR;
		}
	(void)pthread_mutex_lock(&lk);
	while (*st != 2)
		(void)pthread_cond_wait(&cv, &lk);
	(void)pthread_mutex_unlock(&lk);
	return 0;
}
