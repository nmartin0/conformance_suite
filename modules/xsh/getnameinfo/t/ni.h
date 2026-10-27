/*
 * ni.h - helpers shared by the getnameinfo() tests.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * Numeric flags are always used for the host and the service, so no name
 * service is consulted.
 */
#ifndef NI_H
#define NI_H
#include "pc.h"
#include <errno.h>
#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#define NUMF (NI_NUMERICHOST | NI_NUMERICSERV)

static struct sockaddr_in mk4(const unsigned char a[4], unsigned port)
{
	struct sockaddr_in s;
	unsigned char p[2];

	memset(&s, 0, sizeof s);
	s.sin_family = AF_INET;
	memcpy(&s.sin_addr, a, 4);
	p[0] = (unsigned char)(port >> 8);
	p[1] = (unsigned char)(port & 255);
	memcpy(&s.sin_port, p, 2);
	return s;
}

static struct sockaddr_in6 mk6(const unsigned char a[16], unsigned port)
{
	struct sockaddr_in6 s;
	unsigned char p[2];

	memset(&s, 0, sizeof s);
	s.sin6_family = AF_INET6;
	memcpy(&s.sin6_addr, a, 16);
	p[0] = (unsigned char)(port >> 8);
	p[1] = (unsigned char)(port & 255);
	memcpy(&s.sin6_port, p, 2);
	return s;
}

static int terminated(const char *b, size_t n)
{
	return memchr(b, 0, n) != NULL;
}

static unsigned long digits_value(const char *s, int *ok)
{
	char *end;
	unsigned long v;

	*ok = s[0] >= '0' && s[0] <= '9';
	v = strtoul(s, &end, 10);
	if (*end != '\0')
		*ok = 0;
	return v;
}

static void need_v6(void)
{
	int fd = socket(AF_INET6, SOCK_STREAM, 0);

	if (fd < 0)
		pc_notinuse("no IPv6 sockets here (%s)", pc_errname(errno));
	(void)close(fd);
}
#endif
