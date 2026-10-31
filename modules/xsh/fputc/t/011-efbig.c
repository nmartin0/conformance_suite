/*
 * fputc() beyond the file size limit fails with EFBIG.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.fputc.011
 * MUTANT    fputc.errno_lost
 * MUTANT    fputc.success_on_error
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
	/*
	 * The header need not name the signal outside XSI, but the system may
	 * still send it: with every signal blocked it stays pending.
	 */
	sigset_t all;

	(void)sigxfsz_block;
	sigfillset(&all);
	if (sigprocmask(SIG_BLOCK, &all, NULL) != 0)
		return 98;
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
	pc_begin("XSH.fputc.011");
	{
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

			if (limit_fsize(0) != 0)
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
				if (r == EOF)
					_exit(e != EFBIG ? (e == 0 ? 2 : e < 80 ? 10 + e : 2) : ferror(f) ? 0 : 3);
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
		pc_check(st != 1, "fputc() on the unbuffered stream at the limit succeeded");
		pc_check(st != 4, "fputc('x') returned a value other than 'x' or EOF");
		pc_check(st != 2, "the failing fputc() did not set errno, expected EFBIG");
		pc_check(st < 10 || st >= 90, "the failing fputc() set errno %s, expected EFBIG", pc_errname(st - 10));
		pc_check(st != 3, "the stream's error indicator is not set");
		pc_check(st == 0, "the child exited with %d", st);
	}
	pc_pass();
}
