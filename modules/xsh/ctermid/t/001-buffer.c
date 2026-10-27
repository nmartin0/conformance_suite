/*
 * ctermid(buf) fills a buffer of L_ctermid bytes and returns it.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.ctermid.001
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


#define GUARD_BYTES 32

int main(void)
{
	char raw[L_ctermid + GUARD_BYTES];
	char *r;
	size_t i;

	pc_begin("XSH.ctermid.001");
	memset(raw, 0x5A, sizeof raw);
	pc_test();
	r = ctermid(raw);
	pc_check(r == raw, "ctermid() did not return its argument");
	pc_check(memchr(raw, '\0', L_ctermid) != NULL, "no null byte within the first L_ctermid bytes");
	for (i = L_ctermid; i < sizeof raw; i++)
		pc_check((unsigned char)raw[i] == 0x5A, "byte %zu, past L_ctermid, was modified", i);
	pc_pass();
}
