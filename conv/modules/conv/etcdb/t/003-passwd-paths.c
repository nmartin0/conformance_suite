/*
 * Password file: home directories are absolute, shells empty or absolute.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION CONV.etcdb.003
 */
#include <sys/stat.h>
#include <arpa/inet.h>
#include <ctype.h>
#include <errno.h>
#include <grp.h>
#include <netdb.h>
#include <pwd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include "pc.h"
#include "../../../../lib/cv.h"

int main(void)
{
	char *t = NULL, *p, *l, *f[16];
	int n, line = 0;

	pc_begin("CONV.etcdb.003");
	t = cv_etc("PASSWD", "/etc/passwd");
	pc_test();
	p = t;
	while ((l = cv_nextline(&p)) != NULL) {
		line++;
		if (l[0] == '\0' || l[0] == '#' || l[0] == '+' || l[0] == '-')
			continue;
		n = cv_split(l, ":", 0, f, 16);
		if (n != 7)
			continue;
		pc_check(f[5][0] == '/', "home directory '%s' of '%s' (line %d) is not an absolute path", f[5], f[0], line);
		pc_check(f[6][0] == '\0' || f[6][0] == '/', "shell '%s' of '%s' (line %d) is neither empty nor an absolute path",
		    f[6], f[0], line);
	}
	pc_pass();
}
