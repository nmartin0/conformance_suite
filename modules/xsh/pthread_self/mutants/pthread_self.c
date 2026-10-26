/*
 * pthread_self.c - planted faults in pthread_self(), for mutation checks
 * (CI only).
 *
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * A preload library; faults are named "pthread_self.<fault>" and apply
 * only while PC_PHASE is "test".
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

#define PREFIX "pthread_self."

static pthread_t initial;
static int have_initial;
static __thread int created;	/* set in threads made by pthread_create() */
static unsigned long calls;

__attribute__((constructor)) static void remember_initial(void)
{
	REAL(pthread_self);
	initial = real_pthread_self();
	have_initial = 1;
}

struct start {
	void *(*fn)(void *);
	void *arg;
};

static void *trampoline(void *p)
{
	struct start w = *(struct start *)p;

	free(p);
	created = 1;
	return w.fn(w.arg);
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
	r = real_pthread_create(t, attr, trampoline, w);
	if (r != 0)
		free(w);
	return r;
}

pthread_t pthread_self(void)
{
	REAL(pthread_self);
	const char *m = mutant_of(PREFIX);
	pthread_t t = real_pthread_self();

	if (IS("constant") && have_initial)
		return initial;	/* every thread gets the initial thread's ID */
	if (IS("zero_in_threads") && created) {
		memset(&t, 0, sizeof t);	/* not yet set up in new threads */
		return t;
	}
	if (IS("fresh_each_call")) {
		/* a value that changes from call to call */
		unsigned char *b = (unsigned char *)&t;
		calls++;
		b[0] ^= (unsigned char)(calls & 0xff);
		b[sizeof t - 1] ^= 0x40;
	}
	return t;
}
