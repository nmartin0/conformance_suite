/*
 * A write across the file size limit writes what fits.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.write.006
 * MUTANT    write.fsize_efbig_early
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
		if (fd < 0 || getrlimit(RLIMIT_FSIZE, &rl) != 0)
			_exit(2);
#ifdef SIGXFSZ	/* XSI: without it there is no signal to handle */
		if (sigaction(SIGXFSZ, &sa, NULL) != 0)
			_exit(2);
#else
		/*
		 * Outside XSI the header need not name the signal, but the
		 * system may still send it: blocked, it stays pending.
		 */
		(void)sa;
		{
			sigset_t all;

			sigfillset(&all);
			if (sigprocmask(SIG_BLOCK, &all, NULL) != 0)
				_exit(2);
		}
#endif
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
	if (waitpid(pid, &st, 0) != pid)
		pc_unresolved("waitpid failed");
#ifdef SIGXFSZ
	if (WIFSIGNALED(st) && WTERMSIG(st) == SIGXFSZ)
		pc_fail("the writer was killed by SIGXFSZ although it %s the signal",
		    catch_xfsz ? "caught" : "ignored");
#endif
	if (!WIFEXITED(st) || WEXITSTATUS(st) != 0 ||
	    read(p[0], &o, sizeof o) != (ssize_t)sizeof o)
		pc_unresolved("the writing child could not run its write");
	return o;
}

int main(void)
{
	struct outcome o;

	pc_begin("XSH.write.006");
	pc_make_file("f", "");
	pc_require_space();
	pc_test();
	o = limited_write("f", O_WRONLY, 20, 512, 0);
	pc_check(o.r == 20, "write(512) with room for 20 bytes returned %ld (%s)",
	    o.r, o.r < 0 ? pc_errname(o.e) : "-");
	pc_check(pc_file_size("f") == 20, "the file has %ld bytes", pc_file_size("f"));
	pc_pass();
}
