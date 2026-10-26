/*
 * pthread_create.c - planted faults in pthread_create(), for mutation
 * checks (CI only).
 *
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * A preload library; faults are named "pthread_create.<fault>" and apply
 * only while PC_PHASE is "test". Most wrap the start routine, so that the
 * new thread begins (or ends) differently from what the text requires.
 */
#define _GNU_SOURCE 1
#include <dlfcn.h>
#include <errno.h>
#include <fenv.h>
#include <locale.h>
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

static void nap_ms(long ms)
{
	struct timespec ts = { ms / 1000, (ms % 1000) * 1000000L };

	while (nanosleep(&ts, &ts) == -1 && errno == EINTR)
		;
}

/* ---------- keys, for return_skips_dtors ---------- */

static pthread_key_t keys[256];
static int nkeys;

int pthread_key_create(pthread_key_t *key, void (*dtor)(void *))
{
	REAL(pthread_key_create);
	int r = real_pthread_key_create(key, dtor);

	if (r == 0 && nkeys < 256)
		keys[nkeys++] = *key;
	return r;
}

/* ---------- live threads, for the main() faults ---------- */

static pthread_mutex_t lk = PTHREAD_MUTEX_INITIALIZER;
static int live;

/* ---------- the start routine wrapper ---------- */

struct start {
	void *(*fn)(void *);
	void *arg;
	char fault[32];
	sigset_t pend;
	locale_t loc;
	stack_t ss;
	long long cpu_ns;
	const pthread_attr_t *attr;
};

static __thread long long cpu_offset;

int clock_gettime(clockid_t clk, struct timespec *ts)
{
	REAL(clock_gettime);
	int r = real_clock_gettime(clk, ts);

#ifdef CLOCK_THREAD_CPUTIME_ID
	if (r == 0 && clk == CLOCK_THREAD_CPUTIME_ID && cpu_offset > 0) {
		long long v = (long long)ts->tv_sec * 1000000000LL + ts->tv_nsec + cpu_offset;
		ts->tv_sec = (time_t)(v / 1000000000LL);
		ts->tv_nsec = (long)(v % 1000000000LL);
	}
#endif
	return r;
}

int pthread_getcpuclockid(pthread_t t, clockid_t *cid)
{
	REAL(pthread_getcpuclockid);
	const char *m = mutant_of("pthread_create.");

	if (IS("cpuclock_missing"))
		return ENOENT;
	return real_pthread_getcpuclockid(t, cid);
}

static void *trampoline(void *p)
{
	struct start w = *(struct start *)p;
	const char *m = w.fault;
	sigset_t s;
	void *r;
	int sig;

	free(p);
	if (IS("mask_cleared")) {
		sigemptyset(&s);
		(void)pthread_sigmask(SIG_SETMASK, &s, NULL);
	}
	if (IS("mask_full")) {
		sigfillset(&s);
		(void)pthread_sigmask(SIG_SETMASK, &s, NULL);
	}
	if (IS("pending_copied"))
		for (sig = 1; sig < 65; sig++)
			if (sigismember(&w.pend, sig) == 1)
				(void)pthread_kill(pthread_self(), sig);
	if (IS("locale_inherited") && w.loc != (locale_t)0 && w.loc != LC_GLOBAL_LOCALE)
		(void)uselocale(w.loc);
	if (IS("altstack_inherited") && !(w.ss.ss_flags & SS_DISABLE)) {
		w.ss.ss_flags = 0;
		(void)sigaltstack(&w.ss, NULL);
	}
	if (IS("fenv_reset")) {
		int (*set)(const fenv_t *) = (int (*)(const fenv_t *))dlsym(RTLD_DEFAULT, "fesetenv");
		if (set != NULL)
			(void)set(FE_DFL_ENV);
	}
	if (IS("cpuclock_inherited"))
		cpu_offset = w.cpu_ns;
	if (IS("attr_read_late") && w.attr != NULL) {
		int ds;
		nap_ms(100);	/* reads the attributes after the creator has gone on */
		if (pthread_attr_getdetachstate(w.attr, &ds) == 0 && ds == PTHREAD_CREATE_DETACHED)
			(void)pthread_detach(pthread_self());
	}
	if (IS("arg_null"))
		w.arg = NULL;
	if (IS("calls_twice"))
		(void)w.fn(w.arg);
	r = w.fn(w.arg);
	if (IS("drops_retval"))
		r = NULL;
	if (IS("return_skips_dtors")) {
		int i;
		for (i = 0; i < nkeys; i++)
			(void)pthread_setspecific(keys[i], NULL);
	}
	(void)pthread_mutex_lock(&lk);
	live--;
	(void)pthread_mutex_unlock(&lk);
	return r;
}

int pthread_create(pthread_t *restrict t, const pthread_attr_t *restrict attr,
		   void *(*fn)(void *), void *restrict arg)
{
	REAL(pthread_create);
	const char *m = mutant_of("pthread_create.");
	static pthread_t last;
	static int have_last;
	struct start *w;
	pthread_t got;
	int r;

	if (m == NULL)
		return real_pthread_create(t, attr, fn, arg);
	w = calloc(1, sizeof *w);
	if (w == NULL)
		return EAGAIN;
	w->fn = fn;
	w->arg = arg;
	w->attr = attr;
	strncpy(w->fault, m, sizeof w->fault - 1);
	if (IS("pending_copied"))
		(void)sigpending(&w->pend);
	if (IS("locale_inherited"))
		w->loc = uselocale((locale_t)0);
	if (IS("altstack_inherited") && sigaltstack(NULL, &w->ss) != 0)
		w->ss.ss_flags = SS_DISABLE;
#ifdef CLOCK_THREAD_CPUTIME_ID
	if (IS("cpuclock_inherited")) {
		struct timespec ts;
		if (clock_gettime(CLOCK_THREAD_CPUTIME_ID, &ts) == 0)
			w->cpu_ns = (long long)ts.tv_sec * 1000000000LL + ts.tv_nsec;
	}
#endif
	(void)pthread_mutex_lock(&lk);
	live++;
	(void)pthread_mutex_unlock(&lk);
	r = real_pthread_create(&got, attr, trampoline, w);
	if (r != 0) {
		(void)pthread_mutex_lock(&lk);
		live--;
		(void)pthread_mutex_unlock(&lk);
		free(w);
		if (r == EAGAIN && IS("limit_enomem"))
			return ENOMEM;
		if (r == EPERM && IS("eperm_as_einval"))
			return EINVAL;
		if (r == EPERM && IS("eperm_as_minus1")) {
			errno = EPERM;
			return -1;
		}
		return r;
	}
	*t = got;
	if (IS("stores_creator_id"))
		*t = pthread_self();
	if (IS("stores_last_id")) {
		if (have_last)
			*t = last;
		last = got;
		have_last = 1;
	}
	if (IS("returns_minus1"))
		return -1;
	return 0;
}

/* ---------- return from main() ---------- */

static int (*user_main)(int, char **, char **);

static int main_wrapper(int argc, char **argv, char **envp)
{
	int s = user_main(argc, argv, envp), others;
	const char *m = mutant_of("pthread_create.");

	(void)pthread_mutex_lock(&lk);
	others = live;
	(void)pthread_mutex_unlock(&lk);
	if (others > 0 && IS("main_status_lost"))
		exit(0);	/* the status of main() is lost while threads run */
	if (others > 0 && IS("main_skips_atexit"))
		_exit(s);	/* the process ends without exit()'s cleanup */
	return s;
}

typedef int (*start_main_fn)(int (*)(int, char **, char **), int, char **, void *, void *, void *, void *);

int __libc_start_main(int (*mainp)(int, char **, char **), int argc, char **argv,
		      void *a, void *b, void *c, void *d)
{
	start_main_fn real = (start_main_fn)dlsym(RTLD_NEXT, "__libc_start_main");

	user_main = mainp;
	return real(main_wrapper, argc, argv, a, b, c, d);
}
