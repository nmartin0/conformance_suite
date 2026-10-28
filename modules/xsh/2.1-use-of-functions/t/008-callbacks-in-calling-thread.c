/*
 * Callbacks of qsort, bsearch, pthread_once and raise run in the calling thread.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.2.1-use-of-functions.008
 */
#include "pc.h"
#include <errno.h>
#include <pthread.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

static pthread_t expect;
static volatile int wrong_thread;
static volatile int cmp_calls, init_calls, handler_calls;
static pthread_once_t once_main = PTHREAD_ONCE_INIT, once_other = PTHREAD_ONCE_INIT;
static char problem[160];

static void check_here(void)
{
	if (!pthread_equal(pthread_self(), expect))
		wrong_thread++;
}

static int cmp(const void *a, const void *b)
{
	int x = *(const int *)a, y = *(const int *)b;

	check_here();
	cmp_calls++;
	return (x > y) - (x < y);
}

static void init_routine(void)
{
	check_here();
	init_calls++;
}

static void handler(int s)
{
	(void)s;
	check_here();
	handler_calls++;
}

/* Everything in the list, called from the current thread. */
static void run(pthread_once_t *once, const char *where)
{
	int a[16], k, key = 5, *hit;
	struct timespec ts = { 0, 10000000 };

	expect = pthread_self();
	wrong_thread = 0;
	cmp_calls = init_calls = handler_calls = 0;
	for (k = 0; k < 16; k++)
		a[k] = (k * 11) % 16;
	qsort(a, 16, sizeof a[0], cmp);
	if (cmp_calls == 0 || wrong_thread) {
		snprintf(problem, sizeof problem, "%s: qsort comparison %s", where,
		    cmp_calls == 0 ? "never called" : "ran in another thread");
		return;
	}
	cmp_calls = 0;
	hit = bsearch(&key, a, 16, sizeof a[0], cmp);
	if (hit == NULL || *hit != 5 || cmp_calls == 0 || wrong_thread) {
		snprintf(problem, sizeof problem, "%s: bsearch comparison %s", where,
		    hit == NULL || cmp_calls == 0 ? "never called" : "ran in another thread");
		return;
	}
	if (pthread_once(once, init_routine) != 0 || init_calls != 1 || wrong_thread) {
		snprintf(problem, sizeof problem, "%s: pthread_once routine %s", where,
		    init_calls != 1 ? "was not run once" : "ran in another thread");
		return;
	}
	if (raise(SIGUSR1) != 0) {
		snprintf(problem, sizeof problem, "%s: raise failed", where);
		return;
	}
	for (k = 0; k < 200 && handler_calls == 0; k++)
		(void)nanosleep(&ts, NULL);
	if (handler_calls != 1 || wrong_thread) {
		snprintf(problem, sizeof problem, "%s: the handler of raise() %s", where,
		    handler_calls != 1 ? "did not run once" : "ran in another thread");
		return;
	}
}

static void *thread_main(void *arg)
{
	(void)arg;
	run(&once_other, "in a second thread");
	return NULL;
}

static void on_alarm(int s)
{
	(void)s;
	pc_fail("the test timed out (a call did not return)");
}

int main(void)
{
	struct sigaction sa;
	pthread_t th;

	pc_begin("XSH.2.1-use-of-functions.008");
	memset(&sa, 0, sizeof sa);
	sa.sa_handler = handler;
	sigemptyset(&sa.sa_mask);
	if (sigaction(SIGUSR1, &sa, NULL) != 0)
		pc_unresolved("sigaction failed: %s", pc_errname(errno));
	(void)signal(SIGALRM, on_alarm);
	pc_test();
	(void)alarm(30);
	run(&once_main, "in the main thread");
	pc_check(problem[0] == '\0', "%s", problem);
	if (pthread_create(&th, NULL, thread_main, NULL) != 0)
		pc_unresolved("pthread_create failed");
	(void)pthread_join(th, NULL);
	pc_check(problem[0] == '\0', "%s", problem);
	pc_pass();
}
