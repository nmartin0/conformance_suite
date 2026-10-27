/*
 * A who that cannot be a process ID fails with EINVAL or ESRCH.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.getpriority.011
 */
#include "pc.h"
#include <errno.h>
#include <fcntl.h>
#include <limits.h>
#include <signal.h>
#include <stdarg.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/resource.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/un.h>
#include <sys/wait.h>
#include <pthread.h>
#include <time.h>
#include <unistd.h>

/* After pc_test(): a step that is not under test gives UNRESOLVED, not FAIL. */
#define NEED(cond, ...) do { if (!(cond)) pc_unresolved(__VA_ARGS__); } while (0)
#define MS 1000000LL

/* A call that blocks must not hang the test. Also makes sure SIGCHLD has its default action, so that children can be waited for. */
static void on_guard(int sig)
{
	(void)sig;
	pc_unresolved("a call did not finish within the guard time");
}

static void guard(unsigned secs)
{
	struct sigaction sa;

	memset(&sa, 0, sizeof sa);
	sa.sa_handler = on_guard;
	sigemptyset(&sa.sa_mask);
	NEED(sigaction(SIGALRM, &sa, NULL) == 0, "sigaction failed");
	(void)signal(SIGCHLD, SIG_DFL);
	alarm(secs);
}

static long long mono(void)
{
	struct timespec t;

	NEED(clock_gettime(CLOCK_MONOTONIC, &t) == 0, "clock_gettime failed: %s", pc_errname(errno));
	return (long long)t.tv_sec * 1000000000LL + t.tv_nsec;
}

static long long mono_res(void)
{
	struct timespec r;

	NEED(clock_getres(CLOCK_MONOTONIC, &r) == 0, "clock_getres failed: %s", pc_errname(errno));
	return (long long)r.tv_sec * 1000000000LL + r.tv_nsec;
}

static void mkpipe(int p[2])
{
	NEED(pipe(p) == 0, "pipe failed: %s", pc_errname(errno));
}

static void putbyte(int fd)
{
	NEED(write(fd, "x", 1) == 1, "write failed: %s", pc_errname(errno));
}

static void nap_ms(long ms)
{
	struct timespec ts;

	ts.tv_sec = ms / 1000;
	ts.tv_nsec = (ms % 1000) * 1000000L;
	while (nanosleep(&ts, &ts) == -1 && errno == EINTR)
		;
}

#define CH_FAIL 1
#define CH_UNRES 2

static int cfd = -1;

/* In a child: a message for the parent. */
static void cmsg(const char *fmt, ...)
{
	char b[400];
	va_list ap;

	va_start(ap, fmt);
	(void)vsnprintf(b, sizeof b, fmt, ap);
	va_end(ap);
	if (cfd >= 0 && write(cfd, b, strlen(b)) < 0)
		return;
}

#define CFAIL(...) do { cmsg(__VA_ARGS__); _exit(CH_FAIL); } while (0)
#define CUNRES(...) do { cmsg(__VA_ARGS__); _exit(CH_UNRES); } while (0)
#define CCHECK(c, ...) do { if (!(c)) CFAIL(__VA_ARGS__); } while (0)

/*
 * Runs fn in a child that ends with _exit(fn()). Returns its exit status, or
 * -1 with *sigp set when a signal ended it. msg gets what the child said.
 */
static int run_child(int (*fn)(void), char *msg, size_t msz, int *sigp)
{
	int p[2], st;
	pid_t c;
	size_t off = 0;
	ssize_t k;

	NEED(pipe(p) == 0, "pipe failed: %s", pc_errname(errno));
	c = fork();
	NEED(c != -1, "fork failed: %s", pc_errname(errno));
	if (c == 0) {
		close(p[0]);
		cfd = p[1];
		(void)signal(SIGALRM, SIG_DFL);
		alarm(40);
		_exit(fn());
	}
	close(p[1]);
	while (off < msz - 1 && (k = read(p[0], msg + off, msz - 1 - off)) != 0) {
		if (k < 0) {
			NEED(errno == EINTR, "read failed: %s", pc_errname(errno));
			continue;
		}
		off += (size_t)k;
	}
	msg[off] = '\0';
	close(p[0]);
	while (waitpid(c, &st, 0) == -1)
		NEED(errno == EINTR, "waitpid failed: %s", pc_errname(errno));
	*sigp = 0;
	if (WIFSIGNALED(st)) {
		*sigp = WTERMSIG(st);
		return -1;
	}
	return WEXITSTATUS(st);
}

/* Runs fn in a child and maps its report: 0 passes, CH_FAIL fails, CH_UNRES is UNRESOLVED. */
static void judge(int (*fn)(void))
{
	char msg[400];
	int sig, st;

	st = run_child(fn, msg, sizeof msg, &sig);
	if (sig == SIGALRM)
		pc_unresolved("the child did not finish in time");
	if (sig != 0)
		pc_fail("the child was ended by signal %d. %s", sig, msg);
	if (st == CH_UNRES)
		pc_unresolved("%s", msg);
	if (st != 0)
		pc_fail("%s", msg[0] ? msg : "the child failed");
}

/* Gives up privilege where it can: the user ID of a process with ID 0 is changed to an unlikely one. */
static void drop_privilege(void)
{
	if (geteuid() == 0) {
		(void)setgid(60001);
		(void)setuid(60001);
	}
}

/* A limit value that getrlimit() could not represent. */
static int saved_val(rlim_t v)
{
	return v != RLIM_INFINITY && (v == RLIM_SAVED_MAX || v == RLIM_SAVED_CUR);
}


static int child(void)
{
	int r;

	errno = 0;
	r = getpriority(PRIO_PROCESS, (id_t)-1);
	CCHECK(r == -1 && (errno == EINVAL || errno == ESRCH), "getpriority(PRIO_PROCESS, (id_t)-1) returned %d with errno %s, expected -1 with EINVAL or ESRCH", r, pc_errname(errno));
	errno = 0;
	r = setpriority(PRIO_PROCESS, (id_t)-1, NZERO - 1);
	CCHECK(r == -1 && (errno == EINVAL || errno == ESRCH), "setpriority(PRIO_PROCESS, (id_t)-1, NZERO-1) returned %d with errno %s, expected -1 with EINVAL or ESRCH", r, pc_errname(errno));
	return 0;
}

int main(void)
{
	pc_begin("XSH.getpriority.011");
	guard(60);
	pc_test();
	judge(child);
	pc_pass();
}
