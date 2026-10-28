/*
 * struct termios has the four flag words and the control character array.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.11.2-terminal-parameters.001
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
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <termios.h>
#include <time.h>
#include <unistd.h>


int main(void)
{
	struct termios t;

	pc_begin("XBD.11.2-terminal-parameters.001");
	pc_test();
	pc_check(_Generic(t.c_iflag, tcflag_t: 1, default: 0), "c_iflag is not a tcflag_t");
	pc_check(_Generic(t.c_oflag, tcflag_t: 1, default: 0), "c_oflag is not a tcflag_t");
	pc_check(_Generic(t.c_cflag, tcflag_t: 1, default: 0), "c_cflag is not a tcflag_t");
	pc_check(_Generic(t.c_lflag, tcflag_t: 1, default: 0), "c_lflag is not a tcflag_t");
	pc_check(_Generic(t.c_cc[0], cc_t: 1, default: 0), "the elements of c_cc are not cc_t");
	pc_check(sizeof t.c_cc / sizeof t.c_cc[0] == (size_t)NCCS, "c_cc has %d elements, NCCS is %d", (int)(sizeof t.c_cc / sizeof t.c_cc[0]), (int)NCCS);
	pc_pass();
}
