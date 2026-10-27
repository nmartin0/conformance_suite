/*
 * cfgetispeed() leaves the structure alone.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.cfgetispeed.002
 */
#include "pc.h"
#include <errno.h>
#include <fcntl.h>
#include <limits.h>
#include <poll.h>
#include <signal.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/resource.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <termios.h>
#include <time.h>
#include <unistd.h>

/* After pc_test(): a step that is not under test gives UNRESOLVED, not FAIL. */
#define NEED(cond, ...) do { if (!(cond)) pc_unresolved(__VA_ARGS__); } while (0)

static const speed_t all[] = { B0, B50, B75, B110, B134, B150, B200, B300, B600, B1200, B1800, B2400, B4800, B9600, B19200, B38400 };
#define NALL ((int)(sizeof all / sizeof all[0]))

int main(void)
{
	struct termios t, copy;
	speed_t a, b;
	int i, ok = 0;

	pc_begin("XSH.cfgetispeed.002");
	pc_test();
	for (i = 0; i < NALL; i++) {
		memset(&t, 0, sizeof t);
		if (cfsetispeed(&t, all[i]) != 0)
			continue;
		ok++;
		memcpy(&copy, &t, sizeof t);
		a = cfgetispeed(&t);
		b = cfgetispeed(&t);
		pc_check(a == b, "two calls gave different values (index %d)", i);
		pc_check(memcmp(&copy, &t, sizeof t) == 0, "the structure was modified (index %d)", i);
		pc_check(cfgetispeed(&copy) == a, "a copy of the structure gave another value (index %d)", i);
	}
	NEED(ok > 0, "no rate was accepted");
	pc_pass();
}
