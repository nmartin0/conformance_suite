/*
 * A cancellation request made while a thread is blocked in a function that
 * is a cancellation point wakes the thread, and the function does not
 * return.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.2.9-threads.024
 *
 * Each case calls a function that blocks (an empty pipe, a socket with
 * nothing to receive, a limit 30 s ahead, a child that does not exit, a
 * thread that does not end, a full pipe); 150 ms later the main thread
 * requests cancellation, and the thread must be canceled within 10 s
 * without the function returning. SIGUSR1 is blocked in every thread and
 * never sent.
 */
#include "pccp.h"
#include <fcntl.h>
#include <poll.h>
#include <sys/select.h>
#include <stddef.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <sys/wait.h>
#include <unistd.h>
#ifdef __STDC_NO_THREADS__
int main(void)
{
	pc_begin("XSH.2.9-threads.024");
#ifdef _POSIX_SUBPROFILE
	pc_notinuse("<threads.h> is not provided (a subprofile implementation may define __STDC_NO_THREADS__)");
#else
	pc_test();
	pc_fail("__STDC_NO_THREADS__ is defined, but <threads.h> is required unless _POSIX_SUBPROFILE is defined");
#endif
}
#else
#include <threads.h>

static volatile long sink;
static int bp[2];
static int sv[2];
static int lfd;
static int fp[2];
static pid_t child;
static sigset_t usr1;
static pthread_t jt;
static pthread_mutex_t pm = PTHREAD_MUTEX_INITIALIZER;
static pthread_cond_t pcv = PTHREAD_COND_INITIALIZER;
static mtx_t cm;
static cnd_t ccv;

static void *pt_body(void *a)
{
	(void)a;
	while (!flag_get(10))
		sleep_ms(1);
	return NULL;
}

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

static void s_pjoin(void)
{
	flag_set(10, 0);
	jt = start(pt_body, NULL);
}

static void d_pjoin(void)
{
	flag_set(10, 1);
	(void)pthread_join(jt, NULL);
}

static void s_fullpipe(void)
{
	char buf[4096];
	int fl;

	memset(buf, 'z', sizeof buf);
	NEED(pipe(fp) == 0, "pipe failed");
	fl = fcntl(fp[1], F_GETFL);
	NEED(fl != -1 && fcntl(fp[1], F_SETFL, fl | O_NONBLOCK) == 0, "fcntl failed");
	while (write(fp[1], buf, sizeof buf) > 0)
		;
	NEED(errno == EAGAIN || errno == EWOULDBLOCK, "filling the pipe failed");
	NEED(fcntl(fp[1], F_SETFL, fl) == 0, "fcntl failed");
}

static void d_fullpipe(void)
{
	(void)close(fp[0]);
	(void)close(fp[1]);
}

static void c_read(void) { char b; sink = read(bp[0], &b, 1); }
static void c_recv(void) { char b; sink = recv(sv[0], &b, 1, 0); }
static void c_accept(void) { sink = accept(lfd, NULL, NULL); }
static void c_poll(void) { struct pollfd p; p.fd = bp[0]; p.events = POLLIN; p.revents = 0; sink = poll(&p, 1, 30000); }
static void c_ppoll(void) { struct pollfd p; struct timespec ts = { 30, 0 }; p.fd = bp[0]; p.events = POLLIN; p.revents = 0; sink = ppoll(&p, 1, &ts, NULL); }
static void c_select(void)
{
	fd_set s;
	struct timeval tv = { 30, 0 };

	FD_ZERO(&s);
	FD_SET(bp[0], &s);
	sink = select(bp[0] + 1, &s, NULL, NULL, &tv);
}
static void c_pselect(void)
{
	fd_set s;
	struct timespec ts = { 30, 0 };

	FD_ZERO(&s);
	FD_SET(bp[0], &s);
	sink = pselect(bp[0] + 1, &s, NULL, NULL, &ts, NULL);
}
static void c_sigwait(void) { int sig; sink = sigwait(&usr1, &sig); }
static void c_sigsuspend(void) { sigset_t e; sigemptyset(&e); sink = sigsuspend(&e); }
static void c_pause(void) { sink = pause(); }
static void c_nanosleep(void) { struct timespec ts = { 30, 0 }; sink = nanosleep(&ts, NULL); }
static void c_clock_nanosleep(void) { struct timespec ts = { 30, 0 }; sink = clock_nanosleep(CLOCK_MONOTONIC, 0, &ts, NULL); }
static void c_sleep(void) { sink = sleep(30); }
static void c_wait(void) { int st; sink = wait(&st); }
static void c_waitpid(void) { int st; sink = waitpid(child, &st, 0); }
static void c_waitid(void) { siginfo_t si; sink = waitid(P_PID, (id_t)child, &si, WEXITED); }
static void c_pthread_join(void) { sink = pthread_join(jt, NULL); }
static void c_write(void) { sink = write(fp[1], "x", 1); }
static void c_cond_wait(void) { (void)pthread_mutex_lock(&pm); sink = pthread_cond_wait(&pcv, &pm); }
static void c_cnd_wait(void) { (void)mtx_lock(&cm); sink = cnd_wait(&ccv, &cm); }

static const struct cpcase cases[] = {
	{ "read", NULL, c_read, NULL },
	{ "recv", NULL, c_recv, NULL },
	{ "accept", NULL, c_accept, NULL },
	{ "poll", NULL, c_poll, NULL },
	{ "ppoll", NULL, c_ppoll, NULL },
	{ "select", NULL, c_select, NULL },
	{ "pselect", NULL, c_pselect, NULL },
	{ "sigwait", NULL, c_sigwait, NULL },
	{ "sigsuspend", NULL, c_sigsuspend, NULL },
	{ "pause", NULL, c_pause, NULL },
	{ "nanosleep", NULL, c_nanosleep, NULL },
	{ "clock_nanosleep", NULL, c_clock_nanosleep, NULL },
	{ "sleep", NULL, c_sleep, NULL },
	{ "wait", s_child, c_wait, d_child },
	{ "waitpid", s_child, c_waitpid, d_child },
	{ "waitid", s_child, c_waitid, d_child },
	{ "pthread_join", s_pjoin, c_pthread_join, d_pjoin },
	{ "write (full pipe)", s_fullpipe, c_write, d_fullpipe },
	{ "pthread_cond_wait", NULL, c_cond_wait, NULL },
	{ "cnd_wait", NULL, c_cnd_wait, NULL },
};

int main(void)
{
	char bad[768];
	struct sockaddr_un addr;
	socklen_t alen;
	int n;

	pc_begin("XSH.2.9-threads.024");
	guard(250);
	sigemptyset(&usr1);
	sigaddset(&usr1, SIGUSR1);
	NEED(pthread_sigmask(SIG_BLOCK, &usr1, NULL) == 0, "pthread_sigmask failed");
	NEED(pipe(bp) == 0, "pipe failed");
	NEED(socketpair(AF_UNIX, SOCK_STREAM, 0, sv) == 0, "socketpair failed");
	memset(&addr, 0, sizeof addr);
	addr.sun_family = AF_UNIX;
	strcpy(addr.sun_path, "sock");
	alen = (socklen_t)(offsetof(struct sockaddr_un, sun_path) + strlen("sock") + 1);
	lfd = socket(AF_UNIX, SOCK_STREAM, 0);
	NEED(lfd != -1, "socket failed");
	(void)unlink("sock");
	NEED(bind(lfd, (struct sockaddr *)&addr, alen) == 0, "bind failed");
	NEED(listen(lfd, 4) == 0, "listen failed");
	NEED(mtx_init(&cm, mtx_plain) == thrd_success, "mtx_init failed");
	NEED(cnd_init(&ccv) == thrd_success, "cnd_init failed");
	pc_test();
	n = cp_run(cases, (int)(sizeof cases / sizeof cases[0]), CP_BLOCKED, bad, sizeof bad);
	(void)unlink("sock");
	pc_check(n == 0, "a thread blocked in a cancellation point was not canceled by a request in: %s", bad);
	pc_pass();
}
#endif
