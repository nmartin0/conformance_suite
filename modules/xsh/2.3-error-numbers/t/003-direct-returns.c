/*
 * Functions that return an error number as their value: zero for success,
 * the error number itself on failure.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.2.3-error-numbers.003
 */
#include "pc.h"
#include <errno.h>
#include <pthread.h>
#include <signal.h>
#include <stdlib.h>
#include <string.h>

static int once_count;
static void once_fn(void) { once_count++; }
static void *thread_fn(void *a) { return a; }

/* The call must return exactly 0. */
#define Z(call) do { \
	int r_ = (call); \
	pc_check(r_ == 0, "%s returned %d, not 0", #call, r_); \
} while (0)

int main(void)
{
	pthread_mutex_t m, ec;
	pthread_mutexattr_t ma;
	pthread_cond_t cv;
	pthread_rwlock_t rw;
	pthread_attr_t at;
	pthread_key_t key;
	pthread_once_t once = PTHREAD_ONCE_INIT;
	pthread_t th;
	sigset_t set, old;
	void *mem = NULL, *ret = NULL;
	int r;

	pc_begin("XSH.2.3-error-numbers.003");
	sigemptyset(&set);
	if (pthread_mutexattr_init(&ma) != 0 || pthread_mutexattr_settype(&ma, PTHREAD_MUTEX_ERRORCHECK) != 0)
		pc_unresolved("cannot make an error-checking mutex attribute");
	pc_test();

	Z(pthread_mutex_init(&m, NULL));
	Z(pthread_mutex_lock(&m));
	Z(pthread_mutex_unlock(&m));
	Z(pthread_mutex_destroy(&m));

	/* error-checking type: the default type may be recursive, for which trylock succeeds */
	Z(pthread_mutex_init(&ec, &ma));
	Z(pthread_mutex_lock(&ec));
	r = pthread_mutex_trylock(&ec);
	pc_check(r == EBUSY, "pthread_mutex_trylock of a locked mutex returned %d, not EBUSY (%d)", r, EBUSY);
	Z(pthread_mutex_unlock(&ec));
	r = pthread_mutex_unlock(&ec);
	pc_check(r == EPERM, "unlocking an unlocked error-checking mutex returned %d, not EPERM (%d)", r, EPERM);
	Z(pthread_mutex_destroy(&ec));

	Z(pthread_cond_init(&cv, NULL));
	Z(pthread_cond_destroy(&cv));
	Z(pthread_rwlock_init(&rw, NULL));
	Z(pthread_rwlock_rdlock(&rw));
	Z(pthread_rwlock_unlock(&rw));
	Z(pthread_rwlock_destroy(&rw));
	Z(pthread_attr_init(&at));
	r = pthread_attr_setdetachstate(&at, 99);
	pc_check(r == 0 || r == EINVAL, "pthread_attr_setdetachstate(99) returned %d, not 0 or EINVAL (%d)", r, EINVAL);
	Z(pthread_attr_destroy(&at));
	Z(pthread_key_create(&key, NULL));
	Z(pthread_setspecific(key, &once));
	Z(pthread_key_delete(key));
	Z(pthread_once(&once, once_fn));
	pc_check(once_count == 1, "the pthread_once routine ran %d times", once_count);
	Z(pthread_create(&th, NULL, thread_fn, &once));
	Z(pthread_join(th, &ret));
	pc_check(ret == (void *)&once, "pthread_join gave the wrong value");
	Z(posix_memalign(&mem, 64, 100));
	pc_check(mem != NULL && ((size_t)mem % 64) == 0, "posix_memalign gave a bad block");
	free(mem);
	r = posix_memalign(&mem, 3, 100);
	pc_check(r == EINVAL, "posix_memalign with alignment 3 returned %d, not EINVAL (%d)", r, EINVAL);
	Z(pthread_sigmask(SIG_BLOCK, &set, &old));
	Z(pthread_kill(pthread_self(), 0));
	r = pthread_kill(pthread_self(), -1);
	pc_check(r == EINVAL, "pthread_kill with an invalid signal returned %d, not EINVAL (%d)", r, EINVAL);
	pc_pass();
}
