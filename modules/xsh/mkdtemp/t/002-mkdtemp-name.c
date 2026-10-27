/*
 * mkdtemp() replaces only the six trailing X, with portable characters.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.mkdtemp.002
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

static int portable(char c)
{
	return (c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z') || (c >= '0' && c <= '9') || c == '.' || c == '_' || c == '-';
}

/* The last six bytes of t are portable filename characters. */
static int tail_portable(const char *t)
{
	size_t n = strlen(t), i;

	if (n < 6)
		return 0;
	for (i = n - 6; i < n; i++)
		if (!portable(t[i]))
			return 0;
	return 1;
}


int main(void)
{
	char t[] = "sub/pre-XXXXXX";
	struct stat st;

	pc_begin("XSH.mkdtemp.002");
	pc_make_dir("sub");
	pc_test();
	pc_check(mkdtemp(t) != NULL, "mkdtemp() failed: %s", pc_errname(errno));
	pc_check(strlen(t) == strlen("sub/pre-XXXXXX"), "the length changed: %s", t);
	pc_check(strncmp(t, "sub/pre-", 8) == 0, "the part before the X changed: %s", t);
	pc_check(tail_portable(t), "the replacement is not portable filename characters: %s", t);
	pc_check(lstat(t, &st) == 0 && S_ISDIR(st.st_mode), "no directory at %s", t);
	pc_pass();
}
