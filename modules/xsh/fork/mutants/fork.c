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
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>

typedef pid_t (*fork_fn)(void);
typedef int (*tcreate_fn)(clockid_t, struct sigevent *, timer_t *);
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

/* The last timer the process created, so a fault can "inherit" it. */
static int have_timer;
static clockid_t timer_clock;
static struct sigevent timer_ev;
static timer_t timer_id;

int timer_create(clockid_t c, struct sigevent *ev, timer_t *id)
{
	static tcreate_fn real;
	int r;

	if (real == NULL)
		real = (tcreate_fn)dlsym(RTLD_NEXT, "timer_create");
	r = real(c, ev, id);
	if (r == 0 && ev != NULL) {
		have_timer = 1;
		timer_clock = c;
		timer_ev = *ev;
		timer_id = *id;
	}
	return r;
}

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
	struct itimerspec tleft;
	int keep[64], nkeep = 0, kept_as[64];
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
	if (IS("child_inherits_timers") && (!have_timer || timer_gettime(timer_id, &tleft) != 0))
		cannot_plant(m);
#ifdef FD_CLOFORK
	if (IS("child_keeps_clofork"))
		for (i = 3; i < 64; i++) {
			int fl = fcntl(i, F_GETFD);
			if (fl != -1 && (fl & FD_CLOFORK) && nkeep < 64) {
				int d = fcntl(i, F_DUPFD, 200 + nkeep);
				if (d >= 0) {
					keep[nkeep] = i;
					kept_as[nkeep++] = d;
				}
			}
		}
#else
	(void)keep;
	(void)kept_as;
#endif

	r = f();
	if (r == -1)
		return r;
	if (r > 0) {			/* the parent */
		for (i = 0; i < nkeep; i++)
			(void)close(kept_as[i]);
		if (IS("child_waits_parent")) {
			int st;
			(void)waitpid(r, &st, 0);	/* runs the child to completion first */
		}
		if (IS("child_alarm_kept") && left)
			(void)alarm(left);
		if (IS("parent_gets_self"))
			return getpid();
		return r;
	}
	/* the child */
	in_child = 1;
	for (i = 0; i < nkeep; i++) {
		(void)dup2(kept_as[i], keep[i]);	/* brings an FD_CLOFORK descriptor back */
		(void)close(kept_as[i]);
	}
	if (IS("child_inherits_timers") && have_timer) {
		timer_t t;
		struct sigevent ev = timer_ev;
		if (timer_create(timer_clock, &ev, &t) == 0)
			(void)timer_settime(t, 0, &tleft, NULL);
	}
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
