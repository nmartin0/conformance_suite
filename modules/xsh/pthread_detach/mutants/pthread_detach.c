/*
 * pthread_detach.c - planted faults in pthread_detach(), for mutation
 * checks (CI only).
 *
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * A preload library; faults are named "pthread_detach.<fault>" and apply
 * only while PC_PHASE is "test". While one is active, pthread_create()
 * and pthread_exit() are wrapped to record when each thread's start
 * routine has returned.
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

#define PREFIX "pthread_detach."

/*
 * Threads created while a fault of this module is active: whether each
 * has begun to exit (pthread_exit() called or start routine returned),
 * its value, and whether its start routine has returned.
 */
struct entry {
	pthread_t id;
	int used, exiting, finished;
	void *val;
};

static struct entry tab[256];
static pthread_mutex_t tab_lk = PTHREAD_MUTEX_INITIALIZER;
static pthread_cond_t tab_cv = PTHREAD_COND_INITIALIZER;

static struct entry *find(pthread_t t)
{
	int i;

	for (i = 0; i < 256; i++)
		if (tab[i].used && pthread_equal(tab[i].id, t))
			return &tab[i];
	return NULL;
}

static void mark_exit(pthread_t t, void *v, int finished)
{
	struct entry *e;

	(void)pthread_mutex_lock(&tab_lk);
	e = find(t);
	if (e != NULL) {
		e->exiting = 1;
		e->val = v;
		if (finished)
			e->finished = 1;
		(void)pthread_cond_broadcast(&tab_cv);
	}
	(void)pthread_mutex_unlock(&tab_lk);
}

struct start {
	void *(*fn)(void *);
	void *arg;
	struct entry *e;
};

static void *trampoline(void *p)
{
	struct start w = *(struct start *)p;
	void *r;

	free(p);
	(void)pthread_mutex_lock(&tab_lk);
	w.e->id = pthread_self();
	w.e->used = 1;
	(void)pthread_cond_broadcast(&tab_cv);
	(void)pthread_mutex_unlock(&tab_lk);
	r = w.fn(w.arg);
	mark_exit(pthread_self(), r, 1);
	return r;
}

int pthread_create(pthread_t *restrict t, const pthread_attr_t *restrict attr,
		   void *(*fn)(void *), void *restrict arg)
{
	REAL(pthread_create);
	const char *m = mutant_of(PREFIX);
	struct start *w;
	struct entry *e = NULL;
	int i, r;

	if (m == NULL)
		return real_pthread_create(t, attr, fn, arg);
	(void)pthread_mutex_lock(&tab_lk);
	for (i = 0; i < 256 && e == NULL; i++)
		if (!tab[i].used) {
			e = &tab[i];
			memset(e, 0, sizeof *e);
		}
	(void)pthread_mutex_unlock(&tab_lk);
	w = malloc(sizeof *w);
	if (e == NULL || w == NULL) {
		free(w);
		return real_pthread_create(t, attr, fn, arg);
	}
	w->fn = fn;
	w->arg = arg;
	w->e = e;
	r = real_pthread_create(t, attr, trampoline, w);
	if (r != 0) {
		free(w);
		return r;
	}
	(void)pthread_mutex_lock(&tab_lk);
	e->id = *t;
	e->used = 1;
	(void)pthread_mutex_unlock(&tab_lk);
	return 0;
}

void pthread_exit(void *v)
{
	static void (*real)(void *);

	if (real == NULL)
		real = (void (*)(void *))dlsym(RTLD_NEXT, "pthread_exit");
	if (mutant_of(PREFIX) != NULL)
		mark_exit(pthread_self(), v, 0);
	real(v);
	abort();
}

int pthread_detach(pthread_t t)
{
	REAL(pthread_detach);
	const char *m = mutant_of(PREFIX);
	struct entry *e;
	int fin, r;

	if (m == NULL)
		return real_pthread_detach(t);
	(void)pthread_mutex_lock(&tab_lk);
	e = find(t);
	fin = e != NULL && e->finished;
	(void)pthread_mutex_unlock(&tab_lk);
	if (IS("cancels") && !pthread_equal(t, pthread_self()))
		(void)pthread_cancel(t);	/* ends the thread it detaches */
	if (IS("einval_live") && !fin)
		return EINVAL;
	if (IS("self_edeadlk") && pthread_equal(t, pthread_self()))
		return EDEADLK;
	r = real_pthread_detach(t);
	if (IS("zombie_esrch") && fin)
		return ESRCH;
	if (IS("self_ends") && pthread_equal(t, pthread_self()))
		pthread_exit(NULL);
	return r;
}
