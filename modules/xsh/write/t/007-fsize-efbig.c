/*
 * No room under the file size limit gives EFBIG.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.write.007
 * MUTANT    write.efbig_as_enospc
 */
#include "pc.h"
#include <sys/wait.h>
#include <errno.h>
#include <fcntl.h>
#include <limits.h>
#include <string.h>
#include <unistd.h>
#include <sys/resource.h>
#include <signal.h>

/*
 * RLIMIT_FSIZE also limits the file the test's result goes to, so the
 * limited write runs in a child, which reports over a pipe:
 * the write's return value, its errno, and whether SIGXFSZ arrived.
 */
struct outcome {
	long r;
	int e, xfsz;
};

static volatile sig_atomic_t got_xfsz;

static void on_xfsz(int sig)
{
	(void)sig;
	got_xfsz = 1;
}

static struct outcome limited_write(const char *path, int flags, rlim_t limit,
    size_t n, int catch_xfsz)
{
	struct outcome o;
	int p[2], st;
	pid_t pid;

	if (pipe(p) != 0)
		pc_unresolved("pipe failed");
	pid = fork();
	if (pid == -1)
		pc_unresolved("fork failed");
	if (pid == 0) {
		static char buf[4096];
		struct rlimit rl;
		struct sigaction sa;
		int fd = open(path, flags);

		memset(&sa, 0, sizeof sa);
		sa.sa_handler = catch_xfsz ? on_xfsz : SIG_IGN;
		sigemptyset(&sa.sa_mask);
		if (fd < 0 || sigaction(SIGXFSZ, &sa, NULL) != 0 ||
		    getrlimit(RLIMIT_FSIZE, &rl) != 0)
			_exit(2);
		rl.rlim_cur = limit;
		if (setrlimit(RLIMIT_FSIZE, &rl) != 0)
			_exit(2);
		memset(buf, 'x', sizeof buf);
		o.r = (long)write(fd, buf, n);
		o.e = errno;
		o.xfsz = got_xfsz;
		_exit(write(p[1], &o, sizeof o) == (ssize_t)sizeof o ? 0 : 2);
	}
	(void)close(p[1]);
	if (waitpid(pid, &st, 0) == pid && WIFSIGNALED(st) && WTERMSIG(st) == SIGXFSZ)
		pc_fail("the writer was killed by SIGXFSZ although it %s the signal",
		    catch_xfsz ? "caught" : "ignored");
	if (!WIFEXITED(st) || WEXITSTATUS(st) != 0 ||
	    read(p[0], &o, sizeof o) != (ssize_t)sizeof o)
		pc_unresolved("the writing child could not run its write");
	return o;
}

int main(void)
{
	struct outcome o;

	pc_begin("XSH.write.007");
	pc_make_file("f", "0123456789");
	pc_test();
	o = limited_write("f", O_WRONLY | O_APPEND, 10, 1, 0);
	errno = o.e;	/* the child's errno */
	pc_expect_err(o.r, EFBIG, "write() at the file size limit");
	pc_pass();
}
