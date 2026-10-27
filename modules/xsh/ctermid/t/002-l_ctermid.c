/*
 * L_ctermid is defined and positive.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.ctermid.002
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
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>

/* After pc_test(): a step that is not under test gives UNRESOLVED, not FAIL. */
#define NEED(cond, ...) do { if (!(cond)) pc_unresolved(__VA_ARGS__); } while (0)


int main(void)
{
	pc_begin("XSH.ctermid.002");
	pc_test();
#ifdef L_ctermid
	pc_check(L_ctermid > 0, "L_ctermid is %d", (int)L_ctermid);
#else
	pc_fail("L_ctermid is not defined by <stdio.h>");
#endif
	pc_pass();
}
