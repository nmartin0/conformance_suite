/*
 * Password file: numeric ids are decimal integers and unique.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION CONV.etcdb.002
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

#define MAXE 20000
static char names[MAXE][64];
static unsigned long uids[MAXE];

int main(void)
{
	char *t = NULL, *p, *l, *f[16];
	int n, line = 0, entries = 0, i;
	unsigned long v, g;

	pc_begin("CONV.etcdb.002");
	t = cv_etc("PASSWD", "/etc/passwd");
	pc_test();
	p = t;
	while ((l = cv_nextline(&p)) != NULL) {
		line++;
		if (l[0] == '\0' || l[0] == '#' || l[0] == '+' || l[0] == '-')
			continue;
		n = cv_split(l, ":", 0, f, 16);
		if (n != 7)
			continue;	/* reported by CONV.etcdb.001 */
		pc_check(cv_decimal32(f[2], &v), "uid '%s' of '%s' (line %d) is not a decimal integer", f[2], f[0], line);
		pc_check(cv_decimal32(f[3], &g), "gid '%s' of '%s' (line %d) is not a decimal integer", f[3], f[0], line);
		pc_check(entries < MAXE, "too many entries to check");
		for (i = 0; i < entries; i++) {
			pc_check(uids[i] != v, "uid %lu is used by both '%s' and '%s'", v, names[i], f[0]);
			pc_check(strcmp(names[i], f[0]) != 0, "login name '%s' appears twice", f[0]);
		}
		(void)snprintf(names[entries], sizeof names[0], "%s", f[0]);
		uids[entries++] = v;
	}
	pc_pass();
}
