/*
 * pthread_setcancelstate.c - planted faults in pthread_setcancelstate()
 * and pthread_setcanceltype(), and in how the cancelability state and
 * type rule cancellation, for mutation checks (CI only).
 *
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * A preload library; faults are named "pthread_setcancelstate.<fault>"
 * and apply only while PC_PHASE is "test" (as set by pc.c in this
 * process).
 */
#define _GNU_SOURCE 1
#include <dlfcn.h>
#include <errno.h>
#include <pthread.h>
#include <signal.h>
#include <stdlib.h>
#include <string.h>

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

int pthread_setcancelstate(int state, int *old)
{
	const char *m = mutant_of("pthread_setcancelstate.");
	REAL(pthread_setcancelstate);
	int r;

	if (IS("disable_ignored") && state == PTHREAD_CANCEL_DISABLE)
		state = PTHREAD_CANCEL_ENABLE;	/* a no-op that still reports success */
	r = real_pthread_setcancelstate(state, old);
	if (IS("old_wrong") && r == 0 && old != NULL)
		*old = state;			/* reports the new state as the old one */
	return r;
}

int pthread_setcanceltype(int type, int *old)
{
	const char *m = mutant_of("pthread_setcancelstate.");
	REAL(pthread_setcanceltype);
	int r;

	if (IS("async_ignored") && type == PTHREAD_CANCEL_ASYNCHRONOUS)
		type = PTHREAD_CANCEL_DEFERRED;
	r = real_pthread_setcanceltype(type, old);
	if (IS("type_old_wrong") && r == 0 && old != NULL)
		*old = type;
	return r;
}

/* default_disabled: new threads start with cancelability disabled. */
struct start {
	void *(*fn)(void *);
	void *arg;
};

static void *disabled_start(void *p)
{
	struct start s = *(struct start *)p;
	int old;
	REAL(pthread_setcancelstate);

	free(p);
	(void)real_pthread_setcancelstate(PTHREAD_CANCEL_DISABLE, &old);
	return s.fn(s.arg);
}

int pthread_create(pthread_t *restrict t, const pthread_attr_t *restrict a, void *(*fn)(void *), void *restrict arg)
{
	const char *m = mutant_of("pthread_setcancelstate.");
	REAL(pthread_create);
	struct start *s;
	int e;

	if (!IS("default_disabled") || (s = malloc(sizeof *s)) == NULL)
		return real_pthread_create(t, a, fn, arg);
	s->fn = fn;
	s->arg = arg;
	e = real_pthread_create(t, a, disabled_start, s);
	if (e != 0)
		free(s);
	return e;
}

/*
 * acts_async: a request is acted on at once, whatever the target's state
 * and type (delivered as a signal whose handler ends the thread as
 * canceled).
 */
static void act_now(int sig)
{
	(void)sig;
	pthread_exit(PTHREAD_CANCELED);
}

int pthread_cancel(pthread_t t)
{
	const char *m = mutant_of("pthread_setcancelstate.");
	REAL(pthread_cancel);
	struct sigaction sa;

	if (!IS("acts_async"))
		return real_pthread_cancel(t);
	memset(&sa, 0, sizeof sa);
	sa.sa_handler = act_now;
	sigemptyset(&sa.sa_mask);
	(void)sigaction(SIGRTMIN + 4, &sa, NULL);
	return pthread_kill(t, SIGRTMIN + 4);
}
