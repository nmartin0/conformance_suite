/*
 * Hosts file: an address followed by one or more names.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION CONV.etcdb.008
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

/* Is s an IPv4 or IPv6 address in text form (an IPv6 scope after % ignored)? */
static int address(const char *s)
{
	char buf[128], *pc;
	unsigned char a[16];

	(void)snprintf(buf, sizeof buf, "%s", s);
	if ((pc = strchr(buf, '%')) != NULL)
		*pc = '\0';
	return inet_pton(AF_INET, buf, a) == 1 || inet_pton(AF_INET6, buf, a) == 1;
}

int main(void)
{
	char *t = NULL, *p, *l, *f[64];
	int n, line = 0, entries = 0, i;

	pc_begin("CONV.etcdb.008");
	t = cv_etc("HOSTS", "/etc/hosts");
	pc_test();
	p = t;
	while ((l = cv_nextline(&p)) != NULL) {
		line++;
		cv_strip_comment(l);
		if (l[strspn(l, " \t")] == '\0')
			continue;
		entries++;
		n = cv_split(l, " \t", 1, f, 64);
		pc_check(n >= 2, "line %d has an address but no host name", line);
		pc_check(address(f[0]), "'%s' (line %d) is not an IPv4 or IPv6 address", f[0], line);
		for (i = 1; i < n; i++)
			pc_check(strpbrk(f[i], "#\r") == NULL && isprint((unsigned char)f[i][0]),
			    "name '%s' (line %d) has an unprintable or comment character", f[i], line);
	}
	pc_check(entries > 0, "the hosts file has no entries");
	pc_pass();
}
