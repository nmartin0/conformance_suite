/*
 * Services file: name, port/protocol, aliases.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION CONV.etcdb.011
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
	char *t = NULL, *p, *l, *f[64], *slash;
	int n, line = 0, entries = 0;
	unsigned long port;
	size_t k;

	pc_begin("CONV.etcdb.011");
	t = cv_etc("SERVICES", "/etc/services");
	pc_test();
	p = t;
	while ((l = cv_nextline(&p)) != NULL) {
		line++;
		cv_strip_comment(l);
		if (l[strspn(l, " \t")] == '\0')
			continue;
		entries++;
		n = cv_split(l, " \t", 1, f, 64);
		pc_check(n >= 2, "line %d lacks a port/protocol item", line);
		slash = strchr(f[1], '/');
		pc_check(slash != NULL, "'%s' (line %d) is not of the form port/protocol", f[1], line);
		*slash = '\0';
		pc_check(cv_decimal32(f[1], &port) && port <= 65535, "port '%s' of '%s' (line %d) is not a number up to 65535", f[1], f[0], line);
		pc_check(slash[1] != '\0', "service '%s' (line %d) has an empty protocol name", f[0], line);
		for (k = 0; slash[1 + k] != '\0'; k++)
			pc_check(isgraph((unsigned char)slash[1 + k]) && slash[1 + k] != '/',
			    "protocol '%s' (line %d) is not a plain protocol name", slash + 1, line);
	}
	pc_check(entries > 0, "the services file has no entries");
	pc_pass();
}
