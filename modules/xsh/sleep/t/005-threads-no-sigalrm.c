/*
 * In a multi-threaded process, sleep() leaves SIGALRM alone.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.sleep.005
 * MUTANT    sleep.alarm_replaced
 * MUTANT    sleep.alarm_to_handler
 * MUTANT    sleep.alarm_action_left
 * VARIANT   sleep.late
 * VARIANT   sleep.via_alarm
 */
#include <pthread.h>
#include "pc.h"
#include <errno.h>
#include <poll.h>
#include <signal.h>
#include <string.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>

#define NEED(cond, ...) do { if (!(cond)) pc_unresolved(__VA_ARGS__); } while (0)
#define MS 1000000LL

static long long mono(void)
{
	struct timespec t;

	if (clock_gettime(CLOCK_MONOTONIC, &t) != 0)
		pc_unresolved("clock_gettime(CLOCK_MONOTONIC) failed: %s", pc_errname(errno));
	return (long long)t.tv_sec * 1000000000LL + t.tv_nsec;
}

static long long mono_res(void)
{
	struct timespec r;

	if (clock_getres(CLOCK_MONOTONIC, &r) != 0)
		pc_unresolved("clock_getres(CLOCK_MONOTONIC) failed: %s", pc_errname(errno));
	return (long long)r.tv_sec * 1000000000LL + r.tv_nsec;
}

static volatile sig_atomic_t caught;

static void on_sig(int sig)
{
	(void)sig;
	caught++;
}

static void catch_sig(int sig)
{
	struct sigaction sa;

	memset(&sa, 0, sizeof sa);
	sa.sa_handler = on_sig;
	sigemptyset(&sa.sa_mask);
	NEED(sigaction(sig, &sa, NULL) == 0, "sigaction(%d) failed", sig);
}

/*
 * A child that sends sig to this process after first_ms and then every
 * every_ms, until stop_sender() kills it; it waits with poll().
 */
static pid_t send_repeat(int sig, int first_ms, int every_ms)
{
	pid_t parent = getpid(), c = fork();

	NEED(c != -1, "fork failed: %s", pc_errname(errno));
	if (c == 0) {
		(void)poll(NULL, 0, first_ms);
		for (;;) {
			if (kill(parent, sig) != 0)
				_exit(0);
			(void)poll(NULL, 0, every_ms);
		}
	}
	return c;
}

static void stop_sender(pid_t c)
{
	(void)kill(c, SIGKILL);
	while (waitpid(c, NULL, 0) == -1 && errno == EINTR)
		;
}

static int hold[2];

static void *idle(void *arg)
{
	char ch;

	(void)arg;
	(void)read(hold[0], &ch, 1);
	return NULL;
}

int main(void)
{
	struct sigaction now;
	sigset_t pend;
	pthread_t t;
	unsigned r, left;
	int e;

	pc_begin("XSH.sleep.005");
	NEED(pipe(hold) == 0, "pipe failed");
	e = pthread_create(&t, NULL, idle, NULL);
	NEED(e == 0, "pthread_create failed: %s", pc_errname(e));
	catch_sig(SIGALRM);
	(void)alarm(100);
	pc_test();
	r = sleep(1);
	left = alarm(0);
	(void)write(hold[1], "q", 1);
	(void)pthread_join(t, NULL);
	pc_check(r == 0, "sleep(1) returned %u", r);
	pc_check(caught == 0, "the SIGALRM handler ran during sleep(1)");
	pc_check(left >= 1 && left <= 100, "after sleep(1), alarm(0) returned %u: the alarm(100) is %s", left,
	    left == 0 ? "gone" : "changed");
	NEED(sigaction(SIGALRM, NULL, &now) == 0, "sigaction failed");
	pc_check(!(now.sa_flags & SA_SIGINFO) && now.sa_handler == on_sig, "sleep(1) changed the action of SIGALRM");
	NEED(sigpending(&pend) == 0, "sigpending failed");
	pc_check(sigismember(&pend, SIGALRM) != 1, "SIGALRM is pending after sleep(1)");
	pc_pass();
}
