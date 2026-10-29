/*
 * command -v sh names an executable file.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION CONV.layout.008
 */
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "pc.h"
#include "../../../../lib/cv.h"

int main(void)
{
	FILE *p;
	char line[1024];
	struct stat st;
	size_t n;

	pc_begin("CONV.layout.008");
	p = popen("command -v sh", "r");
	pc_check(p != NULL, "cannot run a shell");
	pc_test();
	pc_check(fgets(line, sizeof line, p) != NULL, "command -v sh printed nothing");
	(void)pclose(p);
	n = strlen(line);
	while (n > 0 && line[n - 1] == '\n')
		line[--n] = '\0';
	pc_check(line[0] == '/', "command -v sh printed '%s', not an absolute pathname", line);
	pc_check(stat(line, &st) == 0 && S_ISREG(st.st_mode), "%s is not a regular file", line);
	pc_check(access(line, X_OK) == 0, "%s is not executable", line);
	pc_pass();
}
