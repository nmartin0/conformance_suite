/*
 * The superuser's login shell is listed in the shells file.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION CONV.etcdb.007
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

static int listed(char *shells, const char *shell)
{
	char *p = shells, *l;

	while ((l = cv_nextline(&p)) != NULL) {
		cv_strip_comment(l);
		l += strspn(l, " \t");
		if (l[0] != '\0' && strcmp(l, shell) == 0)
			return 1;
	}
	return 0;
}

int main(void)
{
	char *pw, *sh, *p, *l, *f[16];
	unsigned long v;
	int n, seen = 0;

	pc_begin("CONV.etcdb.007");
	pw = cv_etc("PASSWD", "/etc/passwd");
	sh = cv_etc("SHELLS", "/etc/shells");
	pc_test();
	p = pw;
	while ((l = cv_nextline(&p)) != NULL) {
		if (l[0] == '\0' || l[0] == '#' || l[0] == '+' || l[0] == '-')
			continue;
		n = cv_split(l, ":", 0, f, 16);
		if (n != 7 || !cv_decimal32(f[2], &v) || v != 0 || f[6][0] == '\0')
			continue;
		seen++;
		pc_check(listed(sh, f[6]), "shell %s of uid-0 account '%s' is not listed in the shells file", f[6], f[0]);
	}
	if (seen == 0)
		pc_notinuse("no uid-0 account names a shell");
	pc_pass();
}
