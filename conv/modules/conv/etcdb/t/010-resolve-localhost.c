/*
 * getaddrinfo("localhost") returns loopback addresses, consistent with the hosts file.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION CONV.etcdb.010
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

static int is_loop(const struct sockaddr *sa)
{
	static const unsigned char one[16] = { [15] = 1 };

	if (sa->sa_family == AF_INET)
		return (ntohl(((const struct sockaddr_in *)sa)->sin_addr.s_addr) >> 24) == 127;
	if (sa->sa_family == AF_INET6)
		return memcmp(&((const struct sockaddr_in6 *)sa)->sin6_addr, one, 16) == 0;
	return 0;
}

int main(void)
{
	struct addrinfo hints, *res, *r;
	char *t = NULL, *p, *l, *f[64];
	int n, i, rc, got = 0, filehas = 0, match = 0;

	pc_begin("CONV.etcdb.010");
	t = cv_etc("HOSTS", "/etc/hosts");
	if (cv_env("HOSTS") != NULL)
		pc_notinuse("the hosts file is overridden: the library reads the real one");
	pc_test();
	memset(&hints, 0, sizeof hints);
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	rc = getaddrinfo("localhost", NULL, &hints, &res);
	pc_check(rc == 0, "getaddrinfo(\"localhost\") failed: %s", gai_strerror(rc));
	for (r = res; r != NULL; r = r->ai_next) {
		got++;
		pc_check(is_loop(r->ai_addr), "getaddrinfo(\"localhost\") returned a non-loopback address");
	}
	pc_check(got > 0, "getaddrinfo(\"localhost\") returned no address");
	p = t;
	while ((l = cv_nextline(&p)) != NULL) {
		unsigned char a[16];
		int fam;

		cv_strip_comment(l);
		n = cv_split(l, " \t", 1, f, 64);
		if (n < 2)
			continue;
		fam = inet_pton(AF_INET, f[0], a) == 1 ? AF_INET : inet_pton(AF_INET6, f[0], a) == 1 ? AF_INET6 : 0;
		if (fam == 0)
			continue;
		for (i = 1; i < n; i++) {
			if (strcmp(f[i], "localhost") != 0)
				continue;
			filehas++;
			for (r = res; r != NULL; r = r->ai_next) {
				if (r->ai_family == AF_INET && fam == AF_INET &&
				    memcmp(&((struct sockaddr_in *)r->ai_addr)->sin_addr, a, 4) == 0)
					match++;
				if (r->ai_family == AF_INET6 && fam == AF_INET6 &&
				    memcmp(&((struct sockaddr_in6 *)r->ai_addr)->sin6_addr, a, 16) == 0)
					match++;
			}
		}
	}
	if (filehas > 0)
		pc_check(match > 0, "none of the file's addresses for localhost is among the %d returned", got);
	freeaddrinfo(res);
	pc_pass();
}
