/*
 * Group file: four fields, decimal gids, unique names and gids, comma-separated members.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION CONV.etcdb.004
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
static unsigned long gids[MAXE];

int main(void)
{
	char *t = NULL, *p, *l, *f[16];
	int n, line = 0, entries = 0, i;
	unsigned long v;
	int bad;

	pc_begin("CONV.etcdb.004");
	t = cv_etc("GROUP", "/etc/group");
	pc_test();
	p = t;
	while ((l = cv_nextline(&p)) != NULL) {
		line++;
		if (l[0] == '\0' || l[0] == '#' || l[0] == '+' || l[0] == '-')
			continue;
		n = cv_split(l, ":", 0, f, 16);
		pc_check(n == 4, "line %d has %d colon-separated fields, not 4", line, n);
		pc_check(f[0][0] != '\0' && strpbrk(f[0], " \t\r") == NULL, "group name '%s' (line %d) is empty or has white space", f[0], line);
		pc_check(cv_decimal32(f[2], &v), "gid '%s' of '%s' (line %d) is not a decimal integer", f[2], f[0], line);
		pc_check(strpbrk(f[3], " \t\r") == NULL, "member list of '%s' (line %d) contains white space", f[0], line);
		bad = f[3][0] != '\0' && (f[3][0] == ',' || f[3][strlen(f[3]) - 1] == ',' || strstr(f[3], ",,") != NULL);
		pc_check(!bad, "member list of '%s' (line %d) has an empty name", f[0], line);
		pc_check(entries < MAXE, "too many entries to check");
		for (i = 0; i < entries; i++) {
			pc_check(gids[i] != v, "gid %lu is used by both '%s' and '%s'", v, names[i], f[0]);
			pc_check(strcmp(names[i], f[0]) != 0, "group name '%s' appears twice", f[0]);
		}
		(void)snprintf(names[entries], sizeof names[0], "%s", f[0]);
		gids[entries++] = v;
	}
	pc_check(entries > 0, "the group file has no entries");
	pc_pass();
}
