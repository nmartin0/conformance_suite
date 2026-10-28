/*
 * Functions that are not listed as cancellation points do not act on a
 * pending cancellation request.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.2.9-threads.025
 *
 * A thread with cancelability enabled (deferred) and a request pending
 * calls, one after the other, about fifty functions that are in neither of
 * the standard's two lists of cancellation points; all of them must return
 * and the thread must run on to the end of the list, where
 * pthread_testcancel() then acts on the request (so that the request was
 * pending all along). None of the functions can block.
 */
#include "pcth.h"
#include <fcntl.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/times.h>
#include <semaphore.h>

static int tfd = -1;
static int pp[2], sp[2];
static int sk = -1;
static int dupfd = -1;
static pthread_key_t key;
static sem_t sem;
static pthread_mutex_t mx = PTHREAD_MUTEX_INITIALIZER;
static pthread_rwlock_t rwl = PTHREAD_RWLOCK_INITIALIZER;
static volatile long sink;

static int cmpint(const void *a, const void *b)
{
	return *(const int *)a - *(const int *)b;
}

#define F(name, ...) static void name(void) { __VA_ARGS__; }
F(f_getpid, sink = getpid())
F(f_getppid, sink = getppid())
F(f_getuid, sink = (long)getuid())
F(f_geteuid, sink = (long)geteuid())
F(f_getgid, sink = (long)getgid())
F(f_getegid, sink = (long)getegid())
F(f_getpgrp, sink = getpgrp())
F(f_dup, dupfd = dup(tfd))
F(f_dup2, sink = dup2(tfd, 90))
F(f_pipe, sink = pipe(pp))
F(f_kill, sink = kill(getpid(), 0))
F(f_umask, sink = (long)umask(022))
F(f_chdir, sink = chdir("."))
F(f_time, sink = (long)time(NULL))
F(f_clock_gettime, { struct timespec t; sink = clock_gettime(CLOCK_MONOTONIC, &t); })
F(f_clock_getres, { struct timespec t; sink = clock_getres(CLOCK_MONOTONIC, &t); })
F(f_malloc, { void *p = malloc(100); sink = (long)(p != NULL); free(p); })
F(f_calloc, { void *p = calloc(10, 10); sink = (long)(p != NULL); free(p); })
F(f_realloc, { void *p = realloc(NULL, 50); p = realloc(p, 500); sink = (long)(p != NULL); free(p); })
F(f_strlen, sink = (long)strlen("abc"))
F(f_memcpy, { char a[8], b[8] = "1234567"; memcpy(a, b, 8); sink = memcmp(a, b, 8); })
F(f_sigaction, { struct sigaction sa; sink = sigaction(SIGUSR2, NULL, &sa); })
F(f_sigprocmask, { sigset_t s; sink = sigprocmask(SIG_BLOCK, NULL, &s); })
F(f_sigpending, { sigset_t s; sink = sigpending(&s); })
F(f_pthread_self, sink = (long)pthread_equal(pthread_self(), pthread_self()))
F(f_mutex, { sink = pthread_mutex_lock(&mx); sink = pthread_mutex_unlock(&mx); })
F(f_trylock, { sink = pthread_mutex_trylock(&mx); sink = pthread_mutex_unlock(&mx); })
F(f_tryrdlock, { sink = pthread_rwlock_tryrdlock(&rwl); sink = pthread_rwlock_unlock(&rwl); })
F(f_sem, { sink = sem_post(&sem); sink = sem_trywait(&sem); })
F(f_sched_yield, sink = sched_yield())
F(f_sysconf, sink = sysconf(_SC_PAGESIZE))
F(f_socket, sk = socket(AF_UNIX, SOCK_STREAM, 0))
F(f_socketpair, sink = socketpair(AF_UNIX, SOCK_STREAM, 0, sp))
F(f_getsockname, { struct sockaddr_storage ss; socklen_t l = sizeof ss; sink = getsockname(sk, (struct sockaddr *)&ss, &l); })
F(f_setsockopt, { int v = 1; sink = setsockopt(sk, SOL_SOCKET, SO_REUSEADDR, &v, sizeof v); })
F(f_getsockopt, { int v; socklen_t l = sizeof v; sink = getsockopt(sk, SOL_SOCKET, SO_TYPE, &v, &l); })
F(f_shutdown, sink = shutdown(sp[0], SHUT_WR))
F(f_ftruncate, sink = ftruncate(tfd, 4))
F(f_rmdir, sink = rmdir("no-such-directory"))
F(f_alarm, sink = (long)alarm(120))	/* re-arms the guard timer of main() */
F(f_times, { struct tms t; sink = (long)times(&t); })
F(f_pthread_sigmask, { sigset_t s; sink = pthread_sigmask(SIG_BLOCK, NULL, &s); })
F(f_specific, { sink = pthread_setspecific(key, (const void *)&tfd); sink = (long)(pthread_getspecific(key) != NULL); })
F(f_pthread_kill, sink = pthread_kill(pthread_self(), 0))
F(f_gmtime_r, { time_t t = 0; struct tm tm; sink = (long)(gmtime_r(&t, &tm) != NULL); })
F(f_rand_r, { unsigned s = 5; sink = rand_r(&s); })
F(f_strtol, sink = strtol("123", NULL, 10))
F(f_qsort, { int a[4] = { 3, 1, 4, 2 }; qsort(a, 4, sizeof a[0], cmpint); sink = a[0]; })
F(f_isatty, sink = isatty(tfd))
F(f_sigemptyset, { sigset_t s; sink = sigemptyset(&s); })

#define N(fn) { #fn + 2, fn }
static const struct { const char *name; void (*fn)(void); } fns[] = {
	N(f_getpid), N(f_getppid), N(f_getuid), N(f_geteuid), N(f_getgid), N(f_getegid), N(f_getpgrp), N(f_dup), N(f_dup2),
	N(f_pipe), N(f_kill), N(f_umask), N(f_chdir), N(f_time), N(f_clock_gettime), N(f_clock_getres), N(f_malloc),
	N(f_calloc), N(f_realloc), N(f_strlen), N(f_memcpy), N(f_sigaction), N(f_sigprocmask), N(f_sigpending),
	N(f_pthread_self), N(f_mutex), N(f_trylock), N(f_tryrdlock), N(f_sem), N(f_sched_yield), N(f_sysconf),
	N(f_socket), N(f_socketpair), N(f_getsockname), N(f_setsockopt), N(f_getsockopt), N(f_shutdown),
	N(f_ftruncate), N(f_rmdir), N(f_alarm), N(f_times), N(f_pthread_sigmask), N(f_specific),
	N(f_pthread_kill), N(f_gmtime_r), N(f_rand_r), N(f_strtol), N(f_qsort),
	N(f_isatty), N(f_sigemptyset),
};

enum { NFN = sizeof fns / sizeof fns[0] };

static void cleanup(void *a)
{
	(void)a;
	flag_set(3, 1);
}

static void *runner(void *a)
{
	int i, old;

	(void)a;
	pthread_cleanup_push(cleanup, NULL);
	(void)pthread_setcancelstate(PTHREAD_CANCEL_DISABLE, &old);
	flag_set(0, 1);
	while (!flag_get(4))
		sched_yield();
	(void)pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, &old);
	for (i = 0; i < (int)NFN; i++) {
		flag_set(1, i);
		fns[i].fn();
	}
	flag_set(2, 1);
	pthread_testcancel();
	flag_set(5, 1);
	pthread_cleanup_pop(0);
	return NULL;
}

int main(void)
{
	pthread_t t;
	void *st;

	pc_begin("XSH.2.9-threads.025");
	guard(120);
	pc_make_file("f", "0123456789");
	tfd = open("f", O_RDWR);
	NEED(tfd != -1, "open failed");
	NEED(sem_init(&sem, 0, 0) == 0, "sem_init failed");
	NEED(pthread_key_create(&key, NULL) == 0, "pthread_key_create failed");
	pc_test();
	t = start(runner, NULL);
	NEED(wait_flag(0, 1, 10000), "the thread did not start");
	NEED(pthread_cancel(t) == 0, "pthread_cancel failed");
	flag_set(4, 1);
	{
		long end = now_ms() + 20000;

		while (!flag_get(3) && !flag_get(5) && now_ms() < end)
			sleep_ms(1);
	}
	NEED(flag_get(3) || flag_get(5), "the thread ended neither by cancellation nor by running to the end of the list within 20 s");
	pc_check(flag_get(2) == 1, "the thread was canceled in %s(), which is not a cancellation point", fns[flag_get(1)].name);
	NEED(join_within(t, 10000, &st), "the thread did not end");
	pc_check(flag_get(5) == 0, "pthread_testcancel() returned with a request pending");
	pc_check(st == PTHREAD_CANCELED, "the thread did not end canceled");
	pc_pass();
}
