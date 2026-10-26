/*
 * pthread_create() fails with EPERM when the caller may not use the
 * scheduling policy or parameters it asks for.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.pthread_create.013
 * MUTANT    pthread_create.eperm_as_einval
 * MUTANT    pthread_create.eperm_as_minus1
 */
#include "pc.h"
#include <errno.h>
#include <pthread.h>
#include <sched.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>

static void *start(void *arg)
{
	return arg;
}

/*
 * In the unprivileged child: 0 the thread was created, 1 EPERM, 2 EAGAIN,
 * 3 the attributes could not be set, 10 + n another error number n.
 */
static int attempt(void)
{
	struct sched_param sp;
	pthread_attr_t a;
	pthread_t t;
	int r, prio;

	prio = sched_get_priority_max(SCHED_FIFO);
	if (prio == -1 || pthread_attr_init(&a) != 0
	    || pthread_attr_setinheritsched(&a, PTHREAD_EXPLICIT_SCHED) != 0
	    || pthread_attr_setschedpolicy(&a, SCHED_FIFO) != 0)
		return 3;
	sp.sched_priority = prio;
	if (pthread_attr_setschedparam(&a, &sp) != 0)
		return 3;
	r = pthread_create(&t, &a, start, NULL);
	if (r == 0) {
		(void)pthread_join(t, NULL);
		return 0;
	}
	if (r == EPERM)
		return 1;
	if (r == EAGAIN)
		return 2;
	return 10 + (r > 0 && r < 80 ? r : 80);
}

int main(void)
{
	pid_t pid;
	int st, c;

	pc_begin("XSH.pthread_create.013");
#if defined(_POSIX_THREAD_PRIORITY_SCHEDULING) && _POSIX_THREAD_PRIORITY_SCHEDULING >= 0
	if (sysconf(_SC_THREAD_PRIORITY_SCHEDULING) <= 0)
		pc_unsupported("_SC_THREAD_PRIORITY_SCHEDULING is not supported");
	pc_test();
	pid = fork();
	if (pid == -1)
		pc_unresolved("fork failed: %s", pc_errname(errno));
	if (pid == 0) {
		if (geteuid() == 0 && (setgid(65534) != 0 || setuid(65534) != 0))
			_exit(99);
		if (geteuid() == 0)
			_exit(99);
		_exit(attempt());
	}
	while (waitpid(pid, &st, 0) == -1)
		if (errno != EINTR)
			pc_unresolved("waitpid failed: %s", pc_errname(errno));
	if (!WIFEXITED(st) || WEXITSTATUS(st) == 99)
		pc_unresolved("the unprivileged child could not run (status %#x)", (unsigned)st);
	c = WEXITSTATUS(st);
	if (c == 0)
		pc_unresolved("the unprivileged caller was allowed SCHED_FIFO at its highest priority "
			      "(the privileges needed are implementation-defined)");
	if (c == 2)
		pc_unresolved("pthread_create failed with EAGAIN (no resources)");
	if (c == 3)
		pc_unresolved("the scheduling attributes could not be set");
	pc_check(c == 1, "pthread_create failed with %s, expected EPERM", pc_errname(c - 10));
	pc_pass();
#else
	(void)pid; (void)st; (void)c; (void)attempt;
	pc_unsupported("_POSIX_THREAD_PRIORITY_SCHEDULING is not defined");
#endif
}
