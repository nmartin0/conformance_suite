/*
 * pthread_exit.c - planted faults in pthread_exit(), for mutation checks
 * (CI only).
 *
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * A preload library; faults are named "pthread_exit.<fault>" and apply
 * only while PC_PHASE is "test". To reach the cleanup handlers, it
 * follows the C library's push and pop entry points (glibc's
 * __pthread_register_cancel and musl's _pthread_cleanup_push, which the
 * pthread_cleanup_push() and pthread_cleanup_pop() macros call); it
 * also follows key creation, mutex locks, pipes and atexit()
 * registrations, and counts live threads.
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

/* ---------- cleanup handler records ---------- */

static __thread void *bufs[64];
static __thread int nbufs;

#ifdef __GLIBC__
/* glibc: the macros register an unwind buffer whose first pad word links to the previous one. */
void __pthread_register_cancel(__pthread_unwind_buf_t *buf)
{
	static void (*real)(__pthread_unwind_buf_t *);

	if (real == NULL)
		real = (void (*)(__pthread_unwind_buf_t *))dlsym(RTLD_NEXT, "__pthread_register_cancel");
	real(buf);
	if (nbufs < 64)
		bufs[nbufs++] = buf;
}

void __pthread_unregister_cancel(__pthread_unwind_buf_t *buf)
{
	static void (*real)(__pthread_unwind_buf_t *);

	if (real == NULL)
		real = (void (*)(__pthread_unwind_buf_t *))dlsym(RTLD_NEXT, "__pthread_unregister_cancel");
	if (nbufs > 0 && bufs[nbufs - 1] == buf)
		nbufs--;
	real(buf);
}

static void drop_handlers(void)
{
	while (nbufs > 0)
		__pthread_unregister_cancel(bufs[nbufs - 1]);
}

static void keep_innermost(void)
{
	if (nbufs > 1)
		((__pthread_unwind_buf_t *)bufs[nbufs - 1])->__pad[0] = ((__pthread_unwind_buf_t *)bufs[0])->__pad[0];
}
#else
/* musl: the macros link a struct __ptcb through __next. */
void _pthread_cleanup_push(struct __ptcb *cb, void (*f)(void *), void *x)
{
	static void (*real)(struct __ptcb *, void (*)(void *), void *);

	if (real == NULL)
		real = (void (*)(struct __ptcb *, void (*)(void *), void *))dlsym(RTLD_NEXT, "_pthread_cleanup_push");
	real(cb, f, x);
	if (nbufs < 64)
		bufs[nbufs++] = cb;
}

void _pthread_cleanup_pop(struct __ptcb *cb, int run)
{
	static void (*real)(struct __ptcb *, int);

	if (real == NULL)
		real = (void (*)(struct __ptcb *, int))dlsym(RTLD_NEXT, "_pthread_cleanup_pop");
	if (nbufs > 0 && bufs[nbufs - 1] == cb)
		nbufs--;
	real(cb, run);
}

static void drop_handlers(void)
{
	while (nbufs > 0)
		_pthread_cleanup_pop(bufs[nbufs - 1], 0);
}

static void keep_innermost(void)
{
	if (nbufs > 1)
		((struct __ptcb *)bufs[nbufs - 1])->__next = ((struct __ptcb *)bufs[0])->__next;
}
#endif

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

/* ---------- atexit() registrations made after pc_begin() ---------- */

static void (*exit_fns[32])(void *);
static void *exit_args[32];
static int nexit;

static void note_exit_fn(void (*fn)(void *), void *arg)
{
	if (phase[0] != '\0' && nexit < 32) {
		exit_fns[nexit] = fn;
		exit_args[nexit++] = arg;
	}
}

int __cxa_atexit(void (*fn)(void *), void *arg, void *dso)
{
	REAL(__cxa_atexit);

	note_exit_fn(fn, arg);
	return real___cxa_atexit(fn, arg, dso);
}

int atexit(void (*fn)(void))
{
	REAL(atexit);

	note_exit_fn((void (*)(void *))fn, NULL);
	return real_atexit(fn);
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
	if (IS("skip_cleanup"))
		drop_handlers();
	if (IS("innermost_only"))
		keep_innermost();
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
	if (IS("runs_atexit"))
		for (i = nexit - 1; i >= 0; i--)
			exit_fns[i](exit_args[i]);
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
