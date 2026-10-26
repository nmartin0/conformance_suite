/*
 * fputc() beyond the file size limit generates SIGXFSZ.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.fputc.012
 * MUTANT    fputc.no_sigxfsz
 * VARIANT   setvbuf.refuses_modes
 */
#include "pc.h"
#include <errno.h>
#include <fcntl.h>
#include <limits.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/resource.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>

/* After pc_test(): a step that is not under test gives UNRESOLVED, not FAIL. */
#define NEED(cond, ...) do { if (!(cond)) pc_unresolved(__VA_ARGS__); } while (0)

/*
 * In a child whose soft RLIMIT_FSIZE is 4096 bytes: 98 if the limit cannot
 * be set. sigxfsz_block chooses between blocking SIGXFSZ and ignoring it.
 */
static int limit_fsize(int sigxfsz_block)
{
	struct rlimit rl;
#ifdef SIGXFSZ
	sigset_t set;

	if (sigxfsz_block) {
		sigemptyset(&set);
		sigaddset(&set, SIGXFSZ);
		if (sigprocmask(SIG_BLOCK, &set, NULL) != 0)
			return 98;
	} else
		(void)signal(SIGXFSZ, SIG_IGN);
#else
	(void)sigxfsz_block;
#endif
	if (getrlimit(RLIMIT_FSIZE, &rl) != 0 || (rl.rlim_max != RLIM_INFINITY && rl.rlim_max < 4096))
		return 98;
	rl.rlim_cur = 4096;
	if (setrlimit(RLIMIT_FSIZE, &rl) != 0)
		return 98;
	return 0;
}

/* Waits for kid and returns its exit status (UNRESOLVED if it did not exit). */
static int reap(pid_t kid)
{
	int st;

	while (waitpid(kid, &st, 0) == -1)
		if (errno != EINTR)
			pc_unresolved("waitpid failed: %s", pc_errname(errno));
	if (!WIFEXITED(st))
		pc_unresolved("the child did not exit (status %#x)", (unsigned)st);
	return WEXITSTATUS(st);
}

int main(void)
{
	pc_begin("XSH.fputc.012");
	{
#ifndef SIGXFSZ
		pc_unresolved("SIGXFSZ is not defined");
#else

		static char blk[4096];
		pid_t kid;
		int st;

		memset(blk, 'a', sizeof blk);
		{
			int fd = open("f", O_WRONLY | O_CREAT | O_TRUNC, 0666);

			pc_check(fd >= 0 && write(fd, blk, sizeof blk) == (ssize_t)sizeof blk && close(fd) == 0,
				"cannot make a 4096-byte file: %s", pc_errname(errno));
		}
		pc_test();
		kid = fork();
		if (kid == -1)
			pc_unresolved("fork failed: %s", pc_errname(errno));
		if (kid == 0) {
			FILE *f;
			long i, lim;
			int nb, r, e;

			if (limit_fsize(1) != 0)
				_exit(98);
			f = fopen("f", "a");
			if (f == NULL)
				_exit(98);
			nb = setvbuf(f, NULL, _IONBF, 0) == 0;
			lim = nb ? 1 : (4L << 20) + 1;
			for (i = 0; i < lim; i++) {
				errno = 0;
				r = fputc('x', f);
				e = errno;
				if (r == EOF) {
					sigset_t set;

					if (e != EFBIG)
						_exit(96);
					_exit(sigpending(&set) == 0 && sigismember(&set, SIGXFSZ) == 1 ? 0 : 5);
				}
				if (r != 'x')
					_exit(4);
			}
			_exit(nb ? 1 : 97);
		}
		st = reap(kid);
		if (st == 98)
			pc_unresolved("the child could not set the file size limit or open the file");
		if (st == 97)
			pc_unresolved("no fputc() failed within 4 MiB: the stream's buffer never needed flushing");
		if (st == 96)
			pc_unresolved("fputc() at the limit did not fail with EFBIG (XSH.fputc.011)");
		pc_check(st != 1, "fputc() on the unbuffered stream at the limit succeeded (XSH.fputc.011)");
		pc_check(st != 5, "SIGXFSZ is not pending after fputc() failed with EFBIG");
		pc_check(st == 0, "the child exited with %d", st);
#endif
	}
	pc_pass();
}
