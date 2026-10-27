/*
 * accept() allocates the lowest free descriptor.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.accept.003
 */
#include "pc.h"
#include <arpa/inet.h>
#include <errno.h>
#include <fcntl.h>
#include <limits.h>
#include <netinet/in.h>
#include <poll.h>
#include <signal.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/resource.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/un.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>

/* After pc_test(): a step that is not under test gives UNRESOLVED, not FAIL. */
#define NEED(cond, ...) do { if (!(cond)) pc_unresolved(__VA_ARGS__); } while (0)
#define SAME(a, b) (memcmp(&(a), &(b), sizeof(a)) == 0)

const char *en(int r, int e)
{
	return r == -1 ? pc_errname(e) : "success";
}

/* Is e one of up to four errors (0 = unused)? */
int is_err(int e, int a, int b, int c, int d)
{
	return e == a || (b && e == b) || (c && e == c) || (d && e == d);
}

#ifndef ESOCKTNOSUPPORT
#define ESOCKTNOSUPPORT EPROTONOSUPPORT
#endif

/* A call that blocks must not hang the test. SIGPIPE is ignored, SIGCHLD default. */
void on_guard(int sig)
{
	(void)sig;
	pc_unresolved("a call did not finish within the guard time");
}

void guard(unsigned secs)
{
	struct sigaction sa;

	memset(&sa, 0, sizeof sa);
	sa.sa_handler = on_guard;
	sigemptyset(&sa.sa_mask);
	NEED(sigaction(SIGALRM, &sa, NULL) == 0, "sigaction failed");
	(void)signal(SIGCHLD, SIG_DFL);
	(void)signal(SIGPIPE, SIG_IGN);
	alarm(secs);
}

int reap(pid_t k)
{
	int st;

	while (waitpid(k, &st, 0) == -1)
		if (errno != EINTR)
			return -1;
	return WIFEXITED(st) ? WEXITSTATUS(st) : -1;
}

#define SUNLEN (sizeof(((struct sockaddr_un *)0)->sun_path))
static char tracked[8][600];
static int ntracked;

void untrack_all(void)
{
	int i;

	for (i = 0; i < ntracked; i++)
		(void)unlink(tracked[i]);
}

void track(const char *p)
{
	int i;

	for (i = 0; i < ntracked; i++)
		if (strcmp(tracked[i], p) == 0)
			return;
	if (ntracked == 0)
		atexit(untrack_all);
	if (ntracked < 8 && strlen(p) < sizeof tracked[0])
		strcpy(tracked[ntracked++], p);
}

/* An AF_UNIX address for a socket file in the working directory (absolute when it fits). */
void uaddr(struct sockaddr_un *a, const char *base)
{
	char cwd[4096];

	memset(a, 0, sizeof *a);
	a->sun_family = AF_UNIX;
	NEED(getcwd(cwd, sizeof cwd) != NULL, "getcwd failed");
	if (strlen(cwd) + 1 + strlen(base) < SUNLEN) {
		strcpy(a->sun_path, cwd);
		strcat(a->sun_path, "/");
		strcat(a->sun_path, base);
	} else {
		NEED(strlen(base) < SUNLEN, "name too long");
		strcpy(a->sun_path, base);
	}
	track(a->sun_path);
}

/* An AF_UNIX address with the pathname exactly as given (relative or not). */
void urel(struct sockaddr_un *a, const char *path)
{
	memset(a, 0, sizeof *a);
	a->sun_family = AF_UNIX;
	NEED(strlen(path) < SUNLEN, "name too long");
	strcpy(a->sun_path, path);
	track(path);
}

int usock(int type)
{
	int s = socket(AF_UNIX, type, 0);

	NEED(s != -1, "socket(AF_UNIX) failed: %s", pc_errname(errno));
	return s;
}

int ubind(int s, const char *base)
{
	struct sockaddr_un a;

	uaddr(&a, base);
	NEED(bind(s, (struct sockaddr *)&a, sizeof a) == 0, "bind(%s) failed: %s", base, pc_errname(errno));
	return s;
}

/* A listening AF_UNIX stream socket bound to base in the working directory. */
int ulisten(const char *base, int backlog)
{
	int s = ubind(usock(SOCK_STREAM), base);

	NEED(listen(s, backlog) == 0, "listen failed: %s", pc_errname(errno));
	return s;
}

int uconnect(const char *base)
{
	struct sockaddr_un a;
	int c = usock(SOCK_STREAM);

	uaddr(&a, base);
	NEED(connect(c, (struct sockaddr *)&a, sizeof a) == 0, "connect failed: %s", pc_errname(errno));
	return c;
}

/* Leaves slot L free, L+1 open and L+2 free; returns L. */
int make_holes(void)
{
	int lo, fd, mid;

	pc_make_file("holef", "x");
	lo = pc_lowest_free_fd();
	fd = open("holef", O_RDONLY);
	NEED(fd == lo, "cannot fill slot %d", lo);
	mid = fcntl(fd, F_DUPFD, lo + 1);
	NEED(mid == lo + 1, "cannot fill slot %d", lo + 1);
	NEED(close(fd) == 0, "close failed");
	NEED(fcntl(lo + 2, F_GETFD) == -1, "slot %d is not free", lo + 2);
	return lo;
}

int main(void)
{
	int l, c, lo, k;

	pc_begin("XSH.accept.003");
	guard(30);
	l = ulisten("s1", 4);
	c = uconnect("s1");
	lo = make_holes();
	pc_test();
	k = accept(l, NULL, NULL);
	pc_check(k == lo, "got descriptor %d, expected %d (slots %d and %d free)", k, lo, lo, lo + 2);
	(void)c;
	pc_pass();
}
