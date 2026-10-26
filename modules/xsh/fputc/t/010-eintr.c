/*
 * fputc() interrupted by a caught signal before any data is written fails with EINTR.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.fputc.010
 * MUTANT    fputc.errno_lost
 * MUTANT    fputc.restarts
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
 * Asks for an unbuffered stream (before any other operation on it): 1 if
 * setvbuf() honoured the request. Support for the buffering modes is
 * implementation-defined (XSH 2.5), so a refusal is not a failure.
 */
static int unbuffered(FILE *f)
{
	return setvbuf(f, NULL, _IONBF, 0) == 0;
}

/* Calls of a write function the error tests make, up to this many bytes. */
#define PUT_LIMIT (4L << 20)

/* Fills the pipe completely (setup): non-blocking writes until EAGAIN; leaves O_NONBLOCK set. */
static void fill(int w)
{
	static char blk[4096];
	int fl = fcntl(w, F_GETFL);
	ssize_t r;

	pc_check(fl != -1 && fcntl(w, F_SETFL, fl | O_NONBLOCK) == 0, "cannot set O_NONBLOCK");
	while ((r = write(w, blk, sizeof blk)) > 0)
		;
	while ((r = write(w, blk, 1)) > 0)
		;
	pc_check(r == -1 && errno == EAGAIN, "could not fill the pipe");
}

/*
 * SIGALRM, caught without SA_RESTART and re-armed each second; from the
 * second one on it drains rescue_rfd, so that a write restarted after the
 * first signal can finish.
 */
static volatile sig_atomic_t fired;
static int rescue_rfd = -1;

static void on_alarm(int s)
{
	static char junk[65536];

	(void)s;
	if (++fired >= 2 && rescue_rfd >= 0)
		(void)read(rescue_rfd, junk, sizeof junk);
	(void)alarm(1);
}

static void catch_alarm(void)
{
	struct sigaction sa;

	memset(&sa, 0, sizeof sa);
	sa.sa_handler = on_alarm;
	sigemptyset(&sa.sa_mask);
	if (!(sigaction(SIGALRM, &sa, NULL) == 0))
		pc_unresolved("sigaction failed");
}

int main(void)
{
	pc_begin("XSH.fputc.010");
	{
		FILE *f;
		long i, lim;
		int p[2], fl, nb, r = 0, e = 0;

		if (!(pipe(p) == 0))
			pc_unresolved("pipe failed: %s", pc_errname(errno));
		fill(p[1]);
		fl = fcntl(p[1], F_GETFL);
		if (!(fcntl(p[1], F_SETFL, fl & ~O_NONBLOCK) == 0))
			pc_unresolved("fcntl failed: %s", pc_errname(errno));
		catch_alarm();
		rescue_rfd = p[0];
		pc_test();
		f = fdopen(p[1], "w");
		NEED(f != NULL, "fdopen() failed: %s", pc_errname(errno));
		nb = unbuffered(f);
		lim = nb ? 1 : PUT_LIMIT + 1;
		(void)alarm(1);
		for (i = 0; i < lim; i++) {
			errno = 0;
			r = fputc('x', f);
			e = errno;
			if (r == EOF)
				break;
			if (fired >= 2)
				break;	/* only a write restarted after the first alarm can finish now */
		}
		(void)alarm(0);
		(void)fcntl(p[1], F_SETFL, fl | O_NONBLOCK);	/* nothing left can block at exit */
		pc_check(r == EOF || fired < 2, "fputc() returned %d after the second SIGALRM drained the pipe (was the interrupted write restarted?)", r);
		if (r != EOF && !nb)
			pc_unresolved("no fputc() blocked within %ld bytes: the stream's buffer never needed flushing", PUT_LIMIT);
		pc_check(r == EOF, "fputc() on the unbuffered stream returned %d, expected EOF", r);
		pc_check(e == EINTR, "fputc() set errno %s, expected EINTR", pc_errname(e));
		pc_check(ferror(f) != 0, "the stream's error indicator is not set");
	}
	pc_pass();
}
