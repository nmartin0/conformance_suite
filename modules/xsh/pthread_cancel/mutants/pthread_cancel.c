/*
 * pthread_cancel.c - planted faults in pthread_cancel() and in acting on
 * cancellation, for mutation checks (CI only).
 *
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * A preload library; faults are named "pthread_cancel.<fault>" and apply
 * only while PC_PHASE is "test" (as set by pc.c in this process). Threads
 * are tracked through pthread_create() so a fault can tell a thread that
 * has finished. The cleanup-handler fault reaches the handler list
 * through the C library's own registration functions, which the
 * <pthread.h> macros call (cleanup.c).
 */
#define _GNU_SOURCE 1
#include <dlfcn.h>
#include <errno.h>
#include <pthread.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

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

static void sleep_ms(long ms)
{
	struct timespec ts = { ms / 1000, (ms % 1000) * 1000000L };

	while (nanosleep(&ts, &ts) == -1 && errno == EINTR)
		;
}

/* ---- thread tracking ---- */

struct rec {
	pthread_t t;
	int used, done, canceled_after;
	void *(*fn)(void *);
	void *arg;
};

static pthread_mutex_t tab_lock = PTHREAD_MUTEX_INITIALIZER;
static struct rec tab[64];
static pthread_key_t done_key;
static pthread_once_t once = PTHREAD_ONCE_INIT;

static void mark_done(void *p)
{
	(void)pthread_mutex_lock(&tab_lock);
	((struct rec *)p)->done = 1;
	(void)pthread_mutex_unlock(&tab_lock);
}

static void make_key(void)
{
	REAL(pthread_key_create);
	(void)real_pthread_key_create(&done_key, mark_done);
}

static void *tracked(void *p)
{
	struct rec *r = p;
	void *ret;

	(void)pthread_setspecific(done_key, r);
	ret = r->fn(r->arg);
	mark_done(r);
	return ret;
}

static struct rec *find(pthread_t t)
{
	int i;

	for (i = 0; i < 64; i++)
		if (tab[i].used && pthread_equal(tab[i].t, t))
			return &tab[i];
	return NULL;
}

static int is_done(pthread_t t)
{
	struct rec *r;
	int d;

	(void)pthread_mutex_lock(&tab_lock);
	r = find(t);
	d = r != NULL && r->done;
	(void)pthread_mutex_unlock(&tab_lock);
	return d;
}

int pthread_create(pthread_t *restrict t, const pthread_attr_t *restrict a, void *(*fn)(void *), void *restrict arg)
{
	REAL(pthread_create);
	struct rec *r = NULL;
	int i, e;

	(void)pthread_once(&once, make_key);
	(void)pthread_mutex_lock(&tab_lock);
	for (i = 0; i < 64; i++)
		if (!tab[i].used) {
			r = &tab[i];
			memset(r, 0, sizeof *r);
			r->used = 1;
			r->fn = fn;
			r->arg = arg;
			break;
		}
	if (r == NULL) {
		(void)pthread_mutex_unlock(&tab_lock);
		return real_pthread_create(t, a, fn, arg);
	}
	/* The table lock is held until r->t is set, so find() never sees it unset. */
	e = real_pthread_create(t, a, tracked, r);
	if (e == 0)
		r->t = *t;
	else
		r->used = 0;
	(void)pthread_mutex_unlock(&tab_lock);
	return e;
}

int pthread_join(pthread_t t, void **ret)
{
	const char *m = mutant_of("pthread_cancel.");
	REAL(pthread_join);
	struct rec *r;
	int e, zc = 0;

	(void)pthread_mutex_lock(&tab_lock);
	r = find(t);
	if (r != NULL)
		zc = r->canceled_after;
	(void)pthread_mutex_unlock(&tab_lock);
	e = real_pthread_join(t, ret);
	if (e == 0 && r != NULL) {
		(void)pthread_mutex_lock(&tab_lock);
		r->used = 0;
		(void)pthread_mutex_unlock(&tab_lock);
	}
	if (e == 0 && zc && IS("zombie_status") && ret != NULL)
		*ret = PTHREAD_CANCELED;	/* a finished thread reported as canceled */
	return e;
}

/* ---- pthread_cancel() ---- */

int pthread_cancel(pthread_t t)
{
	const char *m = mutant_of("pthread_cancel.");
	REAL(pthread_cancel);
	struct rec *r;
	int e;

	if (IS("noop"))
		return 0;
	if (is_done(t)) {
		if (IS("zombie_esrch"))
			return ESRCH;
		(void)pthread_mutex_lock(&tab_lock);
		if ((r = find(t)) != NULL)
			r->canceled_after = 1;
		(void)pthread_mutex_unlock(&tab_lock);
	}
	e = real_pthread_cancel(t);
	if (IS("waits"))
		while (!is_done(t))	/* returns only once the target has acted on it */
			sleep_ms(1);
	return e;
}

/* ---- thread-specific data destructors ---- */

int pthread_key_create(pthread_key_t *k, void (*d)(void *))
{
	const char *m = mutant_of("pthread_cancel.");
	REAL(pthread_key_create);

	if (IS("no_destructors"))
		d = NULL;
	return real_pthread_key_create(k, d);
}

/* The active fault of this library, for cleanup.c (which cannot include <pthread.h>). */
const char *pthread_cancel_fault(void)
{
	return mutant_of("pthread_cancel.");
}
