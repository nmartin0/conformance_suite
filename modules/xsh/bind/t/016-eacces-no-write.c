/*
 * Binding in a directory without write permission fails with EACCES.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.bind.016
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

/*
 * Run fn in a child without appropriate privileges: as root the child takes
 * user and group 65534 (the working directory is made searchable for it;
 * relative paths need nothing above it). Returns fn's value (0 to 96); a
 * child that cannot drop privilege, still has it, or dies makes the test
 * UNRESOLVED. Call unpriv_setup() first, in the setup phase.
 */
void unpriv_setup(void)
{
	if (geteuid() == 0 && chmod(".", 0711) != 0)
		pc_unresolved("chmod(.) failed: %s", pc_errname(errno));
	pc_make_file(".pc-priv", "");
	if (chmod(".pc-priv", 0) != 0)
		pc_unresolved("chmod(.pc-priv) failed: %s", pc_errname(errno));
}

int groups_dropped(void)
{
	gid_t gs[64];
	int n = getgroups(64, gs), i;

	if (n < 0)
		return 0;
	for (i = 0; i < n; i++)
		if (gs[i] != getegid() && gs[i] != getgid())
			return 0;
	return 1;
}

int unprivileged(int (*fn)(void))
{
	pid_t kid;
	int st;

	kid = fork();
	if (kid == -1)
		pc_unresolved("fork failed: %s", pc_errname(errno));
	if (kid == 0) {
		alarm(20);
		if (geteuid() == 0) {
			(void)pc_drop_groups();
			if (setgid(65534) != 0 || setuid(65534) != 0)
				_exit(99);
		}
		if (geteuid() == 0)
			_exit(99);
		if (!groups_dropped())
			_exit(98);
		if (open(".pc-priv", O_RDONLY) >= 0)
			_exit(97);
		_exit(fn());
	}
	while (waitpid(kid, &st, 0) == -1)
		if (errno != EINTR)
			pc_unresolved("waitpid failed: %s", pc_errname(errno));
	if (WIFEXITED(st) && WEXITSTATUS(st) == 97)
		pc_unresolved("the child still has appropriate privileges (it can read a mode 0 file)");
	if (WIFEXITED(st) && WEXITSTATUS(st) == 98)
		pc_unresolved("the unprivileged child kept supplementary groups");
	if (!WIFEXITED(st) || WEXITSTATUS(st) == 99)
		pc_unresolved("the unprivileged child could not run (status %#x)", (unsigned)st);
	return WEXITSTATUS(st);
}
static int child(void)
{
	struct sockaddr_un a;
	int s = socket(AF_UNIX, SOCK_STREAM, 0), r;

	if (s == -1)
		return 50;
	memset(&a, 0, sizeof a);
	a.sun_family = AF_UNIX;
	strcpy(a.sun_path, "nw/s");
	r = bind(s, (struct sockaddr *)&a, sizeof a);
	if (r == 0)
		return 1;
	return errno == EACCES ? 0 : 2;
}

int main(void)
{
	int r;

	pc_begin("XSH.bind.016");
	guard(30);
	unpriv_setup();
	pc_make_dir("nw");
	NEED(chmod("nw", 0555) == 0, "chmod failed: %s", pc_errname(errno));
	pc_test();
	r = unprivileged(child);
	pc_check(r != 1, "bind() into a directory without write permission succeeded");
	pc_check(r == 0, "bind() into a directory without write permission failed with another error (status %d), expected EACCES", r);
	pc_pass();
}
