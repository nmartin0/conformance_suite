/*
 * getegid() agrees with the id utility.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.getegid.001
 */
#include "pc.h"
#include <errno.h>
#include <fcntl.h>
#include <limits.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>

#ifndef PATH_MAX
#define PATH_MAX 4096
#endif

int main(void)
{
	char line[64], *end;
	unsigned long v;
	FILE *f;
	gid_t g;

	pc_begin("XSH.getegid.001");
	f = popen("id -g", "r");
	if (f == NULL)
		pc_unresolved("popen(\"id -g\") failed: %s", pc_errname(errno));
	if (fgets(line, sizeof line, f) == NULL)
		pc_unresolved("the id utility printed nothing");
	(void)pclose(f);
	v = strtoul(line, &end, 10);
	if (end == line || strcmp(end, "\n") != 0)
		pc_unresolved("the id utility printed \"%s\", not one decimal number", line);
	pc_test();
	g = getegid();
	pc_check((unsigned long)g == v, "getegid() returned %lu, but \"id -g\" prints %lu", (unsigned long)g, v);
	pc_pass();
}
