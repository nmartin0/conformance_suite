/*
 * pthread_exit.c - planted faults in pthread_exit(), for mutation checks
 * (CI only).
 *
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * A preload library; faults are named "pthread_exit.<fault>" and apply
 * only while PC_PHASE is "test". It follows key creation, mutex locks and
 * pipes, and counts live threads. Cleanup handlers are pushed by macros
 * that a standard interface cannot follow, so no fault is planted in them.
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

#define PREFIX "pthread_exit."

/* A new image (after exec) starts with the phase its environment gives. */
__attribute__((constructor)) static void phase_from_env(void)
{
	const char *p = getenv("PC_PHASE");

	if (p != NULL && strlen(p) < sizeof phase)
		strcpy(phase, p);
}

/* ---------- keys ---------- */

static pthread_key_t keys[256];
static void (*dtors[256])(void *);
static int nkeys;

int pthread_key_create(pthread_key_t *key, void (*dtor)(void *))
{
	REAL(pthread_key_create);
	int r = real_pthread_key_create(key, dtor);

	if (r == 0 && nkeys < 256) {
		keys[nkeys] = *key;
		dtors[nkeys++] = dtor;
	}
	return r;
}

/* ---------- mutexes and pipes of this thread ---------- */

static __thread pthread_mutex_t *held[32];
static __thread int nheld;
static __thread int fds[32];
static __thread int nfds;

int pthread_mutex_lock(pthread_mutex_t *mu)
{
	REAL(pthread_mutex_lock);
	int r = real_pthread_mutex_lock(mu);

	if (r == 0 && nheld < 32)
		held[nheld++] = mu;
	return r;
}

int pthread_mutex_unlock(pthread_mutex_t *mu)
{
	REAL(pthread_mutex_unlock);
	int i;

	for (i = nheld - 1; i >= 0; i--)
		if (held[i] == mu) {
			held[i] = held[--nheld];
			break;
		}
	return real_pthread_mutex_unlock(mu);
}

int pipe(int fd[2])
{
	REAL(pipe);
	int r = real_pipe(fd);

	if (r == 0 && nfds < 31) {
		fds[nfds++] = fd[0];
		fds[nfds++] = fd[1];
	}
	return r;
}

/* ---------- live threads ---------- */

static pthread_mutex_t lk = PTHREAD_MUTEX_INITIALIZER;
static int live = 1;	/* the initial thread */

struct start {
	void *(*fn)(void *);
	void *arg;
};

static void *trampoline(void *p)
{
	struct start w = *(struct start *)p;
	void *r;

	free(p);
	r = w.fn(w.arg);
	(void)pthread_mutex_lock(&lk);
	live--;
	(void)pthread_mutex_unlock(&lk);
	return r;
}

int pthread_create(pthread_t *restrict t, const pthread_attr_t *restrict attr,
		   void *(*fn)(void *), void *restrict arg)
{
	REAL(pthread_create);
	struct start *w;
	int r;

	if (mutant_of(PREFIX) == NULL || (w = malloc(sizeof *w)) == NULL)
		return real_pthread_create(t, attr, fn, arg);
	w->fn = fn;
	w->arg = arg;
	(void)pthread_mutex_lock(&lk);
	live++;
	(void)pthread_mutex_unlock(&lk);
	r = real_pthread_create(t, attr, trampoline, w);
	if (r != 0) {
		(void)pthread_mutex_lock(&lk);
		live--;
		(void)pthread_mutex_unlock(&lk);
		free(w);
	}
	return r;
}

/* ---------- pthread_exit() ---------- */

void pthread_exit(void *v)
{
	static void (*real)(void *);
	const char *m = mutant_of(PREFIX);
	static void *first;
	static int have_first;
	int i, left;

	if (real == NULL)
		real = (void (*)(void *))dlsym(RTLD_NEXT, "pthread_exit");
	if (m == NULL)
		real(v);
	if (IS("value_null"))
		v = NULL;
	if (IS("value_of_first")) {
		if (have_first)
			v = first;
		first = v;
		have_first = 1;
	}
	if (IS("dtors_first"))
		for (i = 0; i < nkeys; i++) {
			void *val = pthread_getspecific(keys[i]);
			if (val != NULL && dtors[i] != NULL) {
				(void)pthread_setspecific(keys[i], NULL);
				dtors[i](val);
			}
		}
	if (IS("skip_dtors"))
		for (i = 0; i < nkeys; i++)
			(void)pthread_setspecific(keys[i], NULL);
	if (IS("unlocks_mutexes"))
		while (nheld > 0)
			(void)pthread_mutex_unlock(held[nheld - 1]);
	if (IS("closes_fds"))
		while (nfds > 0)
			(void)close(fds[--nfds]);
	(void)pthread_mutex_lock(&lk);
	left = --live;
	(void)pthread_mutex_unlock(&lk);
	if (left == 0 && IS("last_no_atexit"))
		_exit(0);	/* the process ends without exit()'s cleanup */
	if (left == 0 && IS("last_uses_value"))
		exit((int)(intptr_t)v);
	real(v);
	abort();
}
