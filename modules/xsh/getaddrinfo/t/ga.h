/*
 * ga.h - helpers shared by the getaddrinfo() tests.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * Only numeric host and service strings are used, so no name service is
 * consulted.
 */
#ifndef GA_H
#define GA_H
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

#define NUMH (AI_NUMERICHOST | AI_NUMERICSERV)

static struct addrinfo mkhints(int flags, int fam, int type, int proto)
{
	struct addrinfo h;

	memset(&h, 0, sizeof h);
	h.ai_flags = flags;
	h.ai_family = fam;
	h.ai_socktype = type;
	h.ai_protocol = proto;
	return h;
}

/* getaddrinfo() that must succeed and give a non-empty list. */
static struct addrinfo *gai_ok(const char *node, const char *serv, int flags, int fam, int type, int proto)
{
	struct addrinfo h = mkhints(flags, fam, type, proto), *res = NULL;
	int rc = getaddrinfo(node, serv, &h, &res);

	pc_check(rc == 0, "getaddrinfo(%s, %s) failed: %s", node ? node : "NULL", serv ? serv : "NULL", gai_strerror(rc));
	pc_check(res != NULL, "getaddrinfo() returned 0 with an empty list");
	return res;
}

/* The return value of a call that must fail. */
static int gai_rc(const char *node, const char *serv, int flags, int fam, int type, int proto)
{
	struct addrinfo h = mkhints(flags, fam, type, proto), *res = NULL;
	int rc = getaddrinfo(node, serv, &h, &res);

	if (rc == 0 && res != NULL)
		freeaddrinfo(res);
	return rc;
}

static int count(const struct addrinfo *p)
{
	int n = 0;

	for (; p != NULL; p = p->ai_next)
		n++;
	return n;
}

/* The family, type and protocol of an entry make a socket. */
static void socketable(const struct addrinfo *p, int proto_override)
{
	int fd = socket(p->ai_family, p->ai_socktype, proto_override >= 0 ? proto_override : p->ai_protocol);

	if (fd < 0 && (errno == EMFILE || errno == ENFILE || errno == ENOBUFS || errno == ENOMEM))
		pc_unresolved("socket() failed: %s", pc_errname(errno));
	/* A raw socket may need privilege (socket() EACCES); entries of that type are allowed. */
	if (fd < 0 && p->ai_socktype == SOCK_RAW && (errno == EACCES || errno == EPERM))
		return;
	pc_check(fd >= 0, "socket(%d, %d, %d) failed: %s", p->ai_family, p->ai_socktype,
	    proto_override >= 0 ? proto_override : p->ai_protocol, pc_errname(errno));
	(void)close(fd);
}

static const struct sockaddr_in *sin_of(const struct addrinfo *p)
{
	pc_check(p->ai_family == AF_INET, "entry family %d, expected AF_INET", p->ai_family);
	pc_check(p->ai_addr != NULL, "ai_addr is null");
	pc_check(p->ai_addr->sa_family == AF_INET, "ai_addr family %d, expected AF_INET", (int)p->ai_addr->sa_family);
	pc_check(p->ai_addrlen >= sizeof(struct sockaddr_in), "ai_addrlen %lu is too small for a sockaddr_in",
	    (unsigned long)p->ai_addrlen);
	return (const struct sockaddr_in *)(const void *)p->ai_addr;
}

static const struct sockaddr_in6 *sin6_of(const struct addrinfo *p)
{
	pc_check(p->ai_family == AF_INET6, "entry family %d, expected AF_INET6", p->ai_family);
	pc_check(p->ai_addr != NULL, "ai_addr is null");
	pc_check(p->ai_addr->sa_family == AF_INET6, "ai_addr family %d, expected AF_INET6", (int)p->ai_addr->sa_family);
	pc_check(p->ai_addrlen >= sizeof(struct sockaddr_in6), "ai_addrlen %lu is too small for a sockaddr_in6",
	    (unsigned long)p->ai_addrlen);
	return (const struct sockaddr_in6 *)(const void *)p->ai_addr;
}

/* Port and address bytes of a sockaddr_in, as stored. */
static void v4_bytes(const struct sockaddr_in *s, unsigned char port[2], unsigned char addr[4])
{
	memcpy(port, &s->sin_port, 2);
	memcpy(addr, &s->sin_addr, 4);
}

static int is_wild4(const struct sockaddr_in *s)
{
	unsigned char a[4];

	memcpy(a, &s->sin_addr, 4);
	return a[0] == 0 && a[1] == 0 && a[2] == 0 && a[3] == 0;
}

/* Setup for the IPv6 tests: IPv6 sockets must exist, or the test does not apply. */
static void need_v6(void)
{
	int fd = socket(AF_INET6, SOCK_STREAM, 0);

	if (fd < 0)
		pc_notinuse("no IPv6 sockets here (%s)", pc_errname(errno));
	(void)close(fd);
}

/* A listening TCP socket on the IPv4 wildcard address; the port it got. */
static int listener4(unsigned *port)
{
	struct sockaddr_in a;
	socklen_t len = sizeof a;
	int fd = socket(AF_INET, SOCK_STREAM, 0);

	if (fd < 0)
		pc_unresolved("socket failed: %s", pc_errname(errno));
	memset(&a, 0, sizeof a);
	a.sin_family = AF_INET;
	a.sin_addr.s_addr = htonl(INADDR_ANY);
	if (bind(fd, (struct sockaddr *)&a, sizeof a) != 0)
		pc_unresolved("bind failed: %s", pc_errname(errno));
	if (listen(fd, 4) != 0)
		pc_unresolved("listen failed: %s", pc_errname(errno));
	if (getsockname(fd, (struct sockaddr *)&a, &len) != 0)
		pc_unresolved("getsockname failed: %s", pc_errname(errno));
	*port = ntohs(a.sin_port);
	return fd;
}
#endif
