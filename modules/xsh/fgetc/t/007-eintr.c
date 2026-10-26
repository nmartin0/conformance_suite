/*
 * fgetc() interrupted by a caught signal before any data fails with EINTR.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.fgetc.007
 * MUTANT    fgetc.restarts
 * MUTANT    fgetc.errno_lost
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
 * SIGALRM, caught without SA_RESTART and re-armed each second; from the
 * second one on it writes a byte to rescue_wfd, so that a read restarted
 * after the first signal can finish.
 */
static volatile sig_atomic_t fired;
static int rescue_wfd = -1;

static void on_alarm(int s)
{
	(void)s;
	if (++fired >= 2 && rescue_wfd >= 0)
		(void)write(rescue_wfd, "z", 1);
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
	pc_begin("XSH.fgetc.007");
	{
		FILE *f;
		int p[2], r, e;

		if (!(pipe(p) == 0))
			pc_unresolved("pipe failed: %s", pc_errname(errno));
		catch_alarm();
		rescue_wfd = p[1];
		pc_test();
		f = fdopen(p[0], "r");
		NEED(f != NULL, "fdopen() failed: %s", pc_errname(errno));
		(void)alarm(1);
		errno = 0;
		r = fgetc(f);
		e = errno;
		(void)alarm(0);
		pc_check(r == EOF, "fgetc() returned %d after the signal, expected EOF (was the read restarted?)", r);
		pc_check(e == EINTR, "fgetc() set errno %s, expected EINTR", pc_errname(e));
		pc_check(ferror(f) != 0, "the stream's error indicator is not set");
	}
	pc_pass();
}
