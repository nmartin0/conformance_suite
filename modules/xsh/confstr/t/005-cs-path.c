/*
 * confstr(_CS_PATH) is a PATH that finds standard utilities.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.confstr.005
 * MUTANT    confstr.path_empty
 * MUTANT    confstr.path_wrong_dir
 */
#include "pc.h"
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

/* 1 if some directory of path holds an executable regular file named util. */
static int found(const char *path, const char *util)
{
	char file[8192];
	const char *p = path, *colon;
	struct stat st;
	size_t len;

	for (;;) {
		colon = strchr(p, ':');
		len = colon ? (size_t)(colon - p) : strlen(p);
		if (len > 0 && len + strlen(util) + 2 <= sizeof file) {
			memcpy(file, p, len);
			file[len] = '/';
			strcpy(file + len + 1, util);
			if (stat(file, &st) == 0 && S_ISREG(st.st_mode) && (st.st_mode & 0111) != 0)
				return 1;
		}
		if (colon == NULL)
			return 0;
		p = colon + 1;
	}
}

int main(void)
{
	static const char *const utils[] = { "sh", "env", "getconf", "cat", "ls" };
	char path[8192];
	size_t n, i;

	pc_begin("XSH.confstr.005");
	pc_test();
	n = confstr(_CS_PATH, path, sizeof path);
	pc_check(n > 1, "confstr(_CS_PATH) returned %zu (errno %s): no value", n, pc_errname(errno));
	if (n > sizeof path)
		pc_unresolved("the value of _CS_PATH needs %zu bytes", n);
	for (i = 0; i < sizeof utils / sizeof utils[0]; i++)
		pc_check(found(path, utils[i]), "no directory of _CS_PATH \"%s\" holds an executable %s", path, utils[i]);
	pc_pass();
}
