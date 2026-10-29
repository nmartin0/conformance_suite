/*
 * Resolver configuration: documented directives with well-formed values.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION CONV.etcdb.015
 */
#include <sys/stat.h>
#include <arpa/inet.h>
#include <ctype.h>
#include <errno.h>
#include <netdb.h>
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
	static const char *const known[] = { "nameserver", "domain", "search", "sortlist", "options", "lookup", "family", NULL };
	char *t = NULL, *p, *l, *f[64];
	int n, line = 0, i, ok;
	unsigned char a[16];
	char buf[128], *pc;

	pc_begin("CONV.etcdb.015");
	t = cv_etc("RESOLV", "/etc/resolv.conf");
	pc_test();
	p = t;
	while ((l = cv_nextline(&p)) != NULL) {
		line++;
		l += strspn(l, " \t");
		if (l[0] == '\0' || l[0] == '#' || l[0] == ';')
			continue;
		n = cv_split(l, " \t\r", 1, f, 64);
		ok = 0;
		for (i = 0; known[i] != NULL; i++)
			if (strcmp(f[0], known[i]) == 0)
				ok = 1;
		pc_check(ok, "line %d starts with '%s', which is not a documented directive", line, f[0]);
		pc_check(n >= 2, "directive '%s' (line %d) has no value", f[0], line);
		if (strcmp(f[0], "nameserver") == 0) {
			(void)snprintf(buf, sizeof buf, "%s", f[1]);
			if ((pc = strchr(buf, '%')) != NULL)
				*pc = '\0';
			pc_check(inet_pton(AF_INET, buf, a) == 1 || inet_pton(AF_INET6, buf, a) == 1,
			    "nameserver value '%s' (line %d) is not an IP address", f[1], line);
		}
		if (strcmp(f[0], "domain") == 0)
			pc_check(n == 2, "domain (line %d) takes one name, found %d values", line, n - 1);
	}
	pc_pass();
}
