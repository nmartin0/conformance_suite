/*
 * remove() of a name longer than {NAME_MAX} fails with ENAMETOOLONG.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.remove.014
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
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>

int main(void)
{
	long max;
	char *name;

	pc_begin("XSH.remove.014");
	errno = 0;
	max = pathconf(".", _PC_NAME_MAX);
	if (max == -1 && errno == 0)
		pc_notinuse("{NAME_MAX} is unbounded for this directory");
	pc_check(max > 0, "pathconf(_PC_NAME_MAX) failed");
	name = malloc((size_t)max + 2);
	pc_check(name != NULL, "malloc failed");
	memset(name, 'a', (size_t)max + 1);
	name[max + 1] = '\0';
	pc_test();
	pc_expect_err(remove(name), ENAMETOOLONG, "remove(<NAME_MAX+1 bytes>)");
	pc_pass();
}
