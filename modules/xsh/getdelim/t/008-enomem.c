/*
 * getdelim() that cannot get the memory for a record fails with ENOMEM.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.getdelim.008
 * MUTANT    getdelim.errno_lost
 * MUTANT    getdelim.no_error_indicator
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
	pc_begin("XSH.getdelim.008");
	{
		pid_t kid;
		int fd, st;

		fd = open("big", O_WRONLY | O_CREAT | O_TRUNC, 0666);
		pc_check(fd >= 0, "open(big) failed: %s", pc_errname(errno));
		pc_check(write(fd, "ab:", 3) == 3, "write(big) failed: %s", pc_errname(errno));
		pc_check(ftruncate(fd, 64L << 20) == 0, "ftruncate(big) to 64 MiB failed: %s", pc_errname(errno));
		pc_check(close(fd) == 0, "close failed");
		pc_test();
		kid = fork();
		if (kid == -1)
			pc_unresolved("fork failed: %s", pc_errname(errno));
		if (kid == 0) {
#ifdef RLIMIT_AS
			static void *chunk[4096];
			struct rlimit rl;
			FILE *f;
			char *p = NULL;
			size_t n = 0;
			ssize_t r;
			int k = 0, e;

			f = fopen("big", "r");
			if (f == NULL || getdelim(&p, &n, ':', f) != 3)
				_exit(98);
			if (getrlimit(RLIMIT_AS, &rl) != 0)
				_exit(98);
			if (rl.rlim_max != RLIM_INFINITY && rl.rlim_max < (rlim_t)1 << 30)
				_exit(98);
			rl.rlim_cur = (rlim_t)1 << 30;
			if (setrlimit(RLIMIT_AS, &rl) != 0)
				_exit(98);
			while (k < 4096 && (chunk[k] = malloc(1 << 20)) != NULL)
				k++;
			if (k == 4096 || k < 3)
				_exit(97);
			free(chunk[--k]);
			free(chunk[--k]);
			errno = 0;
			r = getdelim(&p, &n, ':', f);
			e = errno;
			if (r != -1)
				_exit(1);
			if (e != ENOMEM)
				_exit(e == 0 ? 2 : e < 80 ? 10 + e : 2);
			_exit(ferror(f) ? 0 : 3);
#else
			_exit(96);
#endif
		}
		st = reap(kid);
		if (st == 96)
			pc_unresolved("RLIMIT_AS is not defined: no way to limit the memory here");
		if (st == 98)
			pc_unresolved("the child could not set up the stream or RLIMIT_AS");
		if (st == 97)
			pc_unresolved("the child could not use up its address space under RLIMIT_AS");
		pc_check(st != 1, "getdelim() of a 64 MiB record with about 2 MiB free did not return -1");
		pc_check(st != 2, "getdelim() failed without setting errno, expected ENOMEM");
		pc_check(st < 10 || st >= 90, "getdelim() set errno %s, expected ENOMEM", pc_errname(st - 10));
		pc_check(st != 3, "the stream's error indicator is not set after ENOMEM");
		pc_check(st == 0, "the child exited with %d", st);
	}
	pc_pass();
}
