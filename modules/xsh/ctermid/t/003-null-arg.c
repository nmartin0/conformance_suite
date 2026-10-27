/*
 * ctermid(NULL) returns a string.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.ctermid.003
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
	const char *p;

	pc_begin("XSH.ctermid.003");
	pc_test();
	p = ctermid(NULL);
	pc_check(p != NULL, "ctermid(NULL) returned a null pointer");
	pc_check(memchr(p, '\0', 65536) != NULL, "ctermid(NULL) returned a string with no null byte in 64 KiB");
	pc_pass();
}
