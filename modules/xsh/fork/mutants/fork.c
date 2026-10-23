/*
 * fork.c - planted faults in fork() and _Fork(), for mutation checks
 * (CI only).
 *
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * A preload library; faults are named "fork.<fault>" and apply only while
 * PC_PHASE is "test". Most act in the child right after the real fork:
 * they undo something the child must not have, or bring back something
 * it must not inherit. times() and clock_gettime() are interposed too, so
 * the child can be made to report the parent's CPU time.
 */
#define _GNU_SOURCE 1
#include <dlfcn.h>
#include <errno.h>
#include <fcntl.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/times.h>
#include <time.h>
#include <unistd.h>

typedef pid_t (*fork_fn)(void);
typedef clock_t (*times_fn)(struct tms *);
typedef int (*cgt_fn)(clockid_t, struct timespec *);

static const char *mutant(void)
{
	const char *ph = getenv("PC_PHASE");
	const char *m = getenv("PC_MUTANT");
	if (ph == NULL || strcmp(ph, "test") != 0 || m == NULL ||
	    strncmp(m, "fork.", 5) != 0)
		return NULL;
	return m + 5;
}

#define IS(name) (m != NULL && strcmp(m, name) == 0)

static void cannot_plant(const char *m)
{
	fprintf(stderr, "pcmutant: cannot plant fork.%s here\n", m);
}

/* What the parent had, for faults that make the child inherit it. */
static int in_child;
static struct tms parent_tms;
static struct timespec parent_cpu;

static times_fn real_times(void)
{
	static times_fn fn;
	if (fn == NULL)
		fn = (times_fn)dlsym(RTLD_NEXT, "times");
	return fn;
}

static cgt_fn real_cgt(void)
{
	static cgt_fn fn;
	if (fn == NULL)
		fn = (cgt_fn)dlsym(RTLD_NEXT, "clock_gettime");
	return fn;
}

static pid_t do_fork(const char *m, fork_fn f)
{
	unsigned left = 0;
	sigset_t pend;
	pid_t r;
	int i;

	if (IS("child_alarm_kept"))
		left = alarm(0);
	if (IS("child_pending_kept"))
		(void)sigpending(&pend);
	if (IS("times_inherited"))
		(void)real_times()(&parent_tms);
	if (IS("cputime_inherited"))
		(void)real_cgt()(CLOCK_PROCESS_CPUTIME_ID, &parent_cpu);

	r = f();
	if (r == -1)
		return r;
	if (r > 0) {			/* the parent */
		if (IS("child_alarm_kept") && left)
			(void)alarm(left);
		if (IS("parent_gets_self"))
			return getpid();
		return r;
	}
	/* the child */
	in_child = 1;
	if (IS("child_alarm_kept") && left)
		(void)alarm(left);
	if (IS("child_pending_kept"))
		for (i = 1; i < 32; i++)
			if (sigismember(&pend, i) == 1)
				(void)kill(getpid(), i);
	if (IS("child_gets_pid"))
		return getpid();
	if (IS("pgid_is_pid"))
		(void)setpgid(0, 0);
	if (IS("child_reopens_fds")) {
		/* New open file descriptions for the child's descriptors. */
		for (i = 3; i < 64; i++) {
			char path[64];
			struct stat st;
			int fl = fcntl(i, F_GETFL), n;
			if (fl == -1 || fstat(i, &st) != 0 || !S_ISREG(st.st_mode))
				continue;
			snprintf(path, sizeof path, "/proc/self/fd/%d", i);
			n = open(path, fl & (O_ACCMODE | O_APPEND));
			if (n < 0) {
				cannot_plant(m);
				continue;
			}
			(void)dup2(n, i);
			(void)close(n);
		}
	}
	if (IS("child_closes_dirs")) {
		for (i = 3; i < 256; i++) {
			struct stat st;
			if (fstat(i, &st) == 0 && S_ISDIR(st.st_mode))
				(void)close(i);
		}
	}
	if (IS("child_umask_reset"))
		(void)umask(0);
	if (IS("child_sigmask_cleared")) {
		sigset_t none;
		sigemptyset(&none);
		(void)sigprocmask(SIG_SETMASK, &none, NULL);
	}
	if (IS("child_dispositions_reset")) {
		struct sigaction sa;
		memset(&sa, 0, sizeof sa);
		sa.sa_handler = SIG_DFL;
		(void)sigaction(SIGUSR1, &sa, NULL);
	}
	if (IS("child_gets_timer_signal")) {
		/* As if the parent's 0.2 s SIGUSR1 timer had come along. */
		struct sigevent ev;
		struct itimerspec its;
		timer_t t;
		memset(&ev, 0, sizeof ev);
		ev.sigev_notify = SIGEV_SIGNAL;
		ev.sigev_signo = SIGUSR1;
		memset(&its, 0, sizeof its);
		its.it_value.tv_nsec = 200000000;
		if (timer_create(CLOCK_REALTIME, &ev, &t) == 0)
			(void)timer_settime(t, 0, &its, NULL);
	}
	return 0;
}

pid_t fork(void)
{
	static fork_fn real;
	const char *m = mutant();

	if (real == NULL)
		real = (fork_fn)dlsym(RTLD_NEXT, "fork");
	if (m == NULL)
		return real();
	return do_fork(m, real);
}

pid_t _Fork(void)
{
	static fork_fn real;
	const char *m = mutant();

	if (real == NULL)
		real = (fork_fn)dlsym(RTLD_NEXT, "_Fork");
	if (IS("_Fork_runs_handlers"))
		return fork();	/* calls the pthread_atfork() handlers */
	if (m == NULL || real == NULL)
		return real ? real() : -1;
	return do_fork(m, real);
}

clock_t times(struct tms *t)
{
	const char *m = mutant();
	clock_t r = real_times()(t);

	if (r != (clock_t)-1 && in_child && IS("times_inherited")) {
		t->tms_utime += parent_tms.tms_utime;
		t->tms_stime += parent_tms.tms_stime;
		t->tms_cutime += parent_tms.tms_cutime;
		t->tms_cstime += parent_tms.tms_cstime;
	}
	return r;
}

int clock_gettime(clockid_t c, struct timespec *ts)
{
	const char *m = mutant();
	int r = real_cgt()(c, ts);

	if (r == 0 && in_child && c == CLOCK_PROCESS_CPUTIME_ID &&
	    IS("cputime_inherited")) {
		ts->tv_sec += parent_cpu.tv_sec;
		ts->tv_nsec += parent_cpu.tv_nsec;
		if (ts->tv_nsec >= 1000000000L) {
			ts->tv_sec++;
			ts->tv_nsec -= 1000000000L;
		}
	}
	return r;
}
