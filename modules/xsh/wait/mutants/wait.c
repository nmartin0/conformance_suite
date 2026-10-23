/*
 * wait.c - planted faults in wait() and waitpid(), for mutation checks
 * (CI only).
 *
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * A preload library; faults are named "wait.<fault>" and apply only while
 * PC_PHASE is "test".
 */
#define _GNU_SOURCE 1
#include <dlfcn.h>
#include <errno.h>
#include <signal.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>

typedef pid_t (*waitpid_fn)(pid_t, int *, int);

static const char *mutant(void)
{
	const char *ph = getenv("PC_PHASE");
	const char *m = getenv("PC_MUTANT");
	if (ph == NULL || strcmp(ph, "test") != 0 || m == NULL ||
	    strncmp(m, "wait.", 5) != 0)
		return NULL;
	return m + 5;
}

#define IS(name) (m != NULL && strcmp(m, name) == 0)

static waitpid_fn real_waitpid(void)
{
	static waitpid_fn fn;
	if (fn == NULL)
		fn = (waitpid_fn)dlsym(RTLD_NEXT, "waitpid");
	return fn;
}

static pid_t last_pid;
static int last_status;

static pid_t faulty(const char *m, pid_t pid, int *st, int opt)
{
	waitpid_fn f = real_waitpid();
	int s = 0, e;
	pid_t r;

	if (IS("pid_ignored") && pid > 0)
		pid = -1;
	if (IS("zero_as_any") && pid == 0)
		pid = -1;
	if (IS("group_ignored") && pid < -1)
		pid = -1;
	if (IS("nohang_blocks"))
		opt &= ~WNOHANG;
	if (IS("untraced_ignored"))
		opt &= ~WUNTRACED;
#ifdef WCONTINUED
	if (IS("continued_ignored"))
		opt &= ~WCONTINUED;
#endif
	if (IS("stop_reported_always"))
		opt |= WUNTRACED;
	if (IS("einval_ok"))
		opt &= WNOHANG | WUNTRACED
#ifdef WCONTINUED
		    | WCONTINUED
#endif
		    ;
	r = f(pid, &s, opt);
	while (r == -1 && errno == EINTR && IS("eintr_restarts"))
		r = f(pid, &s, opt);
	e = errno;
	if (r == -1) {
		if (IS("not_consumed") && e == ECHILD && last_pid > 0) {
			/* Reports the last child again, as if not consumed. */
			if (st)
				*st = last_status;
			return last_pid;
		}
		if (IS("echild_as_einval") && e == ECHILD)
			e = EINVAL;
		if (IS("eintr_as_echild") && e == EINTR)
			e = ECHILD;
		errno = e;
		return -1;
	}
	if (r > 0) {
		if (IS("slow_when_ready")) {
			struct timespec ts = { 0, 400000000 };
			(void)nanosleep(&ts, NULL);	/* late although status was there */
		}
		if (IS("status_garbage") && WIFEXITED(s))
			s ^= 0x100;	/* flips a bit of the exit status */
		if (IS("signaled_as_exited") && WIFSIGNALED(s))
			s = (WTERMSIG(s) & 0xff) << 8;
		if (IS("wrong_termsig") && WIFSIGNALED(s))
			s ^= 1;
		if (IS("wrong_pid"))
			r++;
		if (IS("sigchld_left"))
			(void)raise(SIGCHLD);
		last_pid = r;
		last_status = s;
	}
	if (st)
		*st = s;
	return r;
}

pid_t waitpid(pid_t pid, int *st, int opt)
{
	const char *m = mutant();

	if (m == NULL)
		return real_waitpid()(pid, st, opt);
	return faulty(m, pid, st, opt);
}

pid_t wait(int *st)
{
	const char *m = mutant();

	if (m == NULL)
		return real_waitpid()(-1, st, 0);
	return faulty(m, -1, st, 0);
}
