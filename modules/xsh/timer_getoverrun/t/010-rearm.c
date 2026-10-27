/*
 * Setting an armed timer resets it and reports the old setting.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.timer_getoverrun.010
 */
#include "pc.h"
#include <errno.h>
#include <fcntl.h>
#include <limits.h>
#include <poll.h>
#include <signal.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/resource.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <termios.h>
#include <time.h>
#include <unistd.h>

/* After pc_test(): a step that is not under test gives UNRESOLVED, not FAIL. */
#define NEED(cond, ...) do { if (!(cond)) pc_unresolved(__VA_ARGS__); } while (0)

static void msleep(int ms)
{
	struct timespec t;

	t.tv_sec = ms / 1000;
	t.tv_nsec = (long)(ms % 1000) * 1000000L;
	while (nanosleep(&t, &t) == -1 && errno == EINTR)
		;
}

/* 1 if fd becomes readable within ms. */
static int wait_in(int fd, int ms)
{
	struct pollfd pf;
	int n;

	pf.fd = fd;
	pf.events = POLLIN;
	pf.revents = 0;
	do
		n = poll(&pf, 1, ms);
	while (n == -1 && errno == EINTR);
	return n > 0 && (pf.revents & POLLIN) != 0;
}

/* Reads what arrives: waits ms for the first byte, then 50 ms of quiet; returns the count. */
static int collect(int fd, char *b, int cap, int ms)
{
	int n = 0;

	while (n < cap && wait_in(fd, n == 0 ? ms : 50)) {
		ssize_t r = read(fd, b + n, (size_t)(cap - n));

		if (r <= 0)
			break;
		n += (int)r;
	}
	return n;
}


#define MS 1000000LL

static long long ns_of(clockid_t id)
{
	struct timespec t;

	if (clock_gettime(id, &t) != 0)
		pc_unresolved("clock_gettime(%d) failed: %s", (int)id, pc_errname(errno));
	return (long long)t.tv_sec * 1000000000LL + t.tv_nsec;
}

static long long res_of(clockid_t id)
{
	struct timespec r;

	if (clock_getres(id, &r) != 0)
		pc_unresolved("clock_getres(%d) failed: %s", (int)id, pc_errname(errno));
	return (long long)r.tv_sec * 1000000000LL + r.tv_nsec;
}

static long long ts_ns(const struct timespec *t)
{
	return (long long)t->tv_sec * 1000000000LL + t->tv_nsec;
}

static struct itimerspec its_of(long long val_ns, long long int_ns)
{
	struct itimerspec i;

	i.it_value.tv_sec = (time_t)(val_ns / 1000000000LL);
	i.it_value.tv_nsec = (long)(val_ns % 1000000000LL);
	i.it_interval.tv_sec = (time_t)(int_ns / 1000000000LL);
	i.it_interval.tv_nsec = (long)(int_ns % 1000000000LL);
	return i;
}

static int its_zero(const struct itimerspec *i)
{
	return i->it_value.tv_sec == 0 && i->it_value.tv_nsec == 0;
}

static void nop_action(int sig, siginfo_t *si, void *uc)
{
	(void)sig;
	(void)si;
	(void)uc;
}

/* sig is blocked and has a do-nothing SA_SIGINFO action: expirations queue with their value. */
static void sig_setup(int sig)
{
	struct sigaction sa;
	sigset_t ss;

	memset(&sa, 0, sizeof sa);
	sa.sa_sigaction = nop_action;
	sa.sa_flags = SA_SIGINFO;
	sigemptyset(&sa.sa_mask);
	if (sigaction(sig, &sa, NULL) != 0)
		pc_unresolved("sigaction failed: %s", pc_errname(errno));
	sigemptyset(&ss);
	sigaddset(&ss, sig);
	if (sigprocmask(SIG_BLOCK, &ss, NULL) != 0)
		pc_unresolved("sigprocmask failed: %s", pc_errname(errno));
}

/* Accepts sig within ms; returns the number, or -1. */
static int wait_sig(int sig, long ms, siginfo_t *si)
{
	sigset_t ss;
	siginfo_t local;
	struct timespec to;
	int rc;

	sigemptyset(&ss);
	sigaddset(&ss, sig);
	to.tv_sec = ms / 1000;
	to.tv_nsec = (ms % 1000) * 1000000L;
	do
		rc = sigtimedwait(&ss, si != NULL ? si : &local, &to);
	while (rc == -1 && errno == EINTR);
	return rc;
}

/* A timer with SIGEV_SIGNAL; a failure of the set-up is UNRESOLVED. */
static timer_t mk(clockid_t clk, int signo, int val)
{
	struct sigevent sev;
	timer_t id;

	memset(&sev, 0, sizeof sev);
	sev.sigev_notify = SIGEV_SIGNAL;
	sev.sigev_signo = signo;
	sev.sigev_value.sival_int = val;
	if (timer_create(clk, &sev, &id) != 0)
		pc_unresolved("timer_create failed: %s", pc_errname(errno));
	return id;
}

static timer_t mk_none(clockid_t clk)
{
	struct sigevent sev;
	timer_t id;

	memset(&sev, 0, sizeof sev);
	sev.sigev_notify = SIGEV_NONE;
	if (timer_create(clk, &sev, &id) != 0)
		pc_unresolved("timer_create failed: %s", pc_errname(errno));
	return id;
}

/* REALTIME may have been set during a measurement; then the result says nothing. */
static void steady(long long rt0, long long mo0, long long rt1, long long mo1)
{
	long long d = (rt1 - rt0) - (mo1 - mo0);

	if (d > 100 * MS || d < -100 * MS)
		pc_unresolved("CLOCK_REALTIME was adjusted during the measurement");
}

int main(void)
{
	struct itimerspec its, old;
	timer_t id;
	long long t0, t1, res, v, iv;

	pc_begin("XSH.timer_getoverrun.010");
	sig_setup(SIGRTMIN);
	id = mk(CLOCK_MONOTONIC, SIGRTMIN, 9);
	res = res_of(CLOCK_MONOTONIC);
	pc_test();
	alarm(40);
	its = its_of(10000 * MS, 4000 * MS);
	NEED(timer_settime(id, 0, &its, NULL) == 0, "timer_settime failed: %s", pc_errname(errno));
	its = its_of(200 * MS, 0);
	memset(&old, 0x5a, sizeof old);
	t0 = ns_of(CLOCK_MONOTONIC);
	pc_check(timer_settime(id, 0, &its, &old) == 0, "timer_settime() failed: %s", pc_errname(errno));
	v = ts_ns(&old.it_value);
	iv = ts_ns(&old.it_interval);
	pc_check(v >= 5000 * MS && v <= 10000 * MS + res, "the old time left is %lld ns", v);
	pc_check(iv >= 4000 * MS && iv <= 4000 * MS + res, "the old interval is %lld ns", iv);
	pc_check(wait_sig(SIGRTMIN, 3000, NULL) == SIGRTMIN, "the reset timer did not expire within 3 s");
	t1 = ns_of(CLOCK_MONOTONIC);
	pc_check(t1 - t0 + res >= 200 * MS, "expired after only %lld ns", t1 - t0);
	pc_check(wait_sig(SIGRTMIN, 500, NULL) == -1, "a second expiration came from a one-shot timer");
	its = its_of(0, 0);
	memset(&old, 0x5a, sizeof old);
	pc_check(timer_settime(id, 0, &its, &old) == 0, "timer_settime() failed: %s", pc_errname(errno));
	pc_check(its_zero(&old) && ts_ns(&old.it_interval) == 0, "the old setting of the disarmed timer is not zero");
	timer_delete(id);
	pc_pass();
}
