/*
 * Password file: seven fields per line, sane login names.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION CONV.etcdb.001
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
	int n, line = 0, entries = 0;

	pc_begin("CONV.etcdb.001");
	t = cv_etc("PASSWD", "/etc/passwd");
	pc_test();
	p = t;
	while ((l = cv_nextline(&p)) != NULL) {
		line++;
		if (l[0] == '\0' || l[0] == '#' || l[0] == '+' || l[0] == '-')
			continue;
		entries++;
		n = cv_split(l, ":", 0, f, 16);
		pc_check(n == 7, "line %d has %d colon-separated fields, not 7", line, n);
		pc_check(f[0][0] != '\0', "line %d has an empty login name", line);
		pc_check(f[0][0] != '-', "login name '%s' (line %d) begins with a hyphen", f[0], line);
		pc_check(strpbrk(f[0], " \t\r") == NULL, "login name '%s' (line %d) contains white space", f[0], line);
	}
	pc_check(entries > 0, "the password file has no entries");
	pc_pass();
}
