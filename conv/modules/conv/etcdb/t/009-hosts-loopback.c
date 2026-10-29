/*
 * Hosts file: an entry gives localhost a loopback address.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION CONV.etcdb.009
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

static int loopback(const char *s)
{
	unsigned char a[16];
	static const unsigned char one[16] = { [15] = 1 };

	if (inet_pton(AF_INET, s, a) == 1)
		return a[0] == 127;
	return inet_pton(AF_INET6, s, a) == 1 && memcmp(a, one, 16) == 0;
}

int main(void)
{
	char *t = NULL, *p, *l, *f[64];
	int n, i, found = 0;

	pc_begin("CONV.etcdb.009");
	t = cv_etc("HOSTS", "/etc/hosts");
	pc_test();
	p = t;
	while ((l = cv_nextline(&p)) != NULL) {
		cv_strip_comment(l);
		n = cv_split(l, " \t", 1, f, 64);
		if (n < 2 || !loopback(f[0]))
			continue;
		for (i = 1; i < n; i++)
			if (strcmp(f[i], "localhost") == 0)
				found = 1;
	}
	pc_check(found, "no line gives the name localhost a loopback address (127.x.y.z or ::1)");
	pc_pass();
}
