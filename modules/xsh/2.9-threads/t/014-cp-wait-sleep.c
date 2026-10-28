/*
 * Sleeping, polling, signal-waiting and child-waiting functions listed as
 * cancellation points act on a pending cancellation request before they
 * return.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.2.9-threads.014
 *
 * For each of nanosleep(), clock_nanosleep(), sleep(), thrd_sleep(),
 * pause(), poll(), ppoll(), select(), pselect(), sigsuspend(),
 * sigtimedwait(), sigwait(), sigwaitinfo(), wait(), waitpid() and
 * waitid(): see XSH.2.9-threads.012. Each call is given a wait of 30 s
 * or no end; SIGUSR1 is blocked in every thread and never sent.
 */
#include "pccp.h"
#include <poll.h>
#include <sys/select.h>
#include <sys/wait.h>
#include <threads.h>

static volatile long sink;
static pid_t child;
static sigset_t usr1;

static void s_child(void)
{
	child = fork();
	if (child == 0) {
		alarm(60);
		for (;;)
			pause();
	}
	NEED(child != -1, "fork failed");
}

static void d_child(void)
{
	int st;

	if (child > 0) {
		(void)kill(child, SIGKILL);
		while (waitpid(child, &st, 0) == -1 && errno == EINTR)
			;
	}
	child = 0;
}

static void c_nanosleep(void) { struct timespec ts = { 30, 0 }; sink = nanosleep(&ts, NULL); }
static void c_clock_nanosleep(void) { struct timespec ts = { 30, 0 }; sink = clock_nanosleep(CLOCK_MONOTONIC, 0, &ts, NULL); }
static void c_sleep(void) { sink = sleep(30); }
static void c_thrd_sleep(void) { struct timespec ts = { 30, 0 }; sink = thrd_sleep(&ts, NULL); }
static void c_pause(void) { sink = pause(); }
static void c_poll(void) { sink = poll(NULL, 0, 30000); }
static void c_ppoll(void) { struct timespec ts = { 30, 0 }; sink = ppoll(NULL, 0, &ts, NULL); }
static void c_select(void) { struct timeval tv = { 30, 0 }; sink = select(0, NULL, NULL, NULL, &tv); }
static void c_pselect(void) { struct timespec ts = { 30, 0 }; sink = pselect(0, NULL, NULL, NULL, &ts, NULL); }
static void c_sigsuspend(void) { sigset_t e; sigemptyset(&e); sink = sigsuspend(&e); }
static void c_sigtimedwait(void) { siginfo_t si; struct timespec ts = { 30, 0 }; sink = sigtimedwait(&usr1, &si, &ts); }
static void c_sigwait(void) { int sig; sink = sigwait(&usr1, &sig); }
static void c_sigwaitinfo(void) { siginfo_t si; sink = sigwaitinfo(&usr1, &si); }
static void c_wait(void) { int st; sink = wait(&st); }
static void c_waitpid(void) { int st; sink = waitpid(child, &st, 0); }
static void c_waitid(void) { siginfo_t si; sink = waitid(P_PID, (id_t)child, &si, WEXITED); }

static const struct cpcase cases[] = {
	{ "nanosleep", NULL, c_nanosleep, NULL },
	{ "clock_nanosleep", NULL, c_clock_nanosleep, NULL },
	{ "sleep", NULL, c_sleep, NULL },
	{ "thrd_sleep", NULL, c_thrd_sleep, NULL },
	{ "pause", NULL, c_pause, NULL },
	{ "poll", NULL, c_poll, NULL },
	{ "ppoll", NULL, c_ppoll, NULL },
	{ "select", NULL, c_select, NULL },
	{ "pselect", NULL, c_pselect, NULL },
	{ "sigsuspend", NULL, c_sigsuspend, NULL },
	{ "sigtimedwait", NULL, c_sigtimedwait, NULL },
	{ "sigwait", NULL, c_sigwait, NULL },
	{ "sigwaitinfo", NULL, c_sigwaitinfo, NULL },
	{ "wait", s_child, c_wait, d_child },
	{ "waitpid", s_child, c_waitpid, d_child },
	{ "waitid", s_child, c_waitid, d_child },
};

int main(void)
{
	char bad[512];
	int n;

	pc_begin("XSH.2.9-threads.014");
	guard(250);
	sigemptyset(&usr1);
	sigaddset(&usr1, SIGUSR1);
	NEED(pthread_sigmask(SIG_BLOCK, &usr1, NULL) == 0, "pthread_sigmask failed");
	pc_test();
	n = cp_run(cases, (int)(sizeof cases / sizeof cases[0]), CP_PENDING, bad, sizeof bad);
	pc_check(n == 0, "with a cancellation request pending and cancelability enabled the thread was not canceled in: %s", bad);
	pc_pass();
}
