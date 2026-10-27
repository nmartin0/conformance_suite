/*
 * A signal that interrupts fgetwc() before any data arrives gives EINTR.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.fgetwc.009
 */
#include "pc.h"
#include <errno.h>
#include <fcntl.h>
#include <limits.h>
#include <locale.h>
#include <signal.h>
#include <stdarg.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>
#include <wchar.h>

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
	pc_begin("XSH.fgetwc.009");
	{
		FILE *f;
		int p[2], e;
		wint_t r;

		if (!(pipe(p) == 0))
			pc_unresolved("pipe failed: %s", pc_errname(errno));
		catch_alarm();
		rescue_wfd = p[1];
		pc_test();
		f = fdopen(p[0], "r");
		NEED(f != NULL, "fdopen() failed: %s", pc_errname(errno));
		(void)alarm(1);
		errno = 0;
		r = fgetwc(f);
		e = errno;
		(void)alarm(0);
		pc_check(r == WEOF, "fgetwc() returned %#lx after the signal, expected WEOF (was the read restarted?)", (unsigned long)r);
		pc_check(e == EINTR, "fgetwc() set errno %s, expected EINTR", pc_errname(e));
		pc_check(ferror(f) != 0, "the stream's error indicator is not set");
	}
	pc_pass();
}
