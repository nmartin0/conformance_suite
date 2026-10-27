/*
 * ip.h - helpers shared by the inet_ntop() and inet_pton() tests.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 */
#ifndef IP_H
#define IP_H
#include "pc.h"
#include <arpa/inet.h>
#include <errno.h>
#include <netinet/in.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>

struct r4 {
	const char *text;
	unsigned char b[4];
};

struct r6 {
	const char *text;
	unsigned char b[16];
};

/* inet_pton() of text for family af must return 1 and the given bytes. */
static void want4(const struct r4 *r)
{
	unsigned char out[4];
	int rc;

	memset(out, 0xA5, sizeof out);
	rc = inet_pton(AF_INET, r->text, out);
	pc_check(rc == 1, "inet_pton(AF_INET, \"%s\") returned %d, expected 1", r->text, rc);
	pc_check(memcmp(out, r->b, 4) == 0, "inet_pton(AF_INET, \"%s\") gave %u.%u.%u.%u", r->text, out[0], out[1], out[2], out[3]);
}

static void want6(const struct r6 *r)
{
	unsigned char out[16];
	int rc;

	memset(out, 0xA5, sizeof out);
	rc = inet_pton(AF_INET6, r->text, out);
	pc_check(rc == 1, "inet_pton(AF_INET6, \"%s\") returned %d, expected 1", r->text, rc);
	pc_check(memcmp(out, r->b, 16) == 0, "inet_pton(AF_INET6, \"%s\") gave other bytes", r->text);
}

static void reject(int af, const char *text)
{
	unsigned char out[16];
	int rc;

	memset(out, 0, sizeof out);
	rc = inet_pton(af, text, out);
	pc_check(rc == 0, "inet_pton(%s, \"%s\") returned %d, expected 0", af == AF_INET ? "AF_INET" : "AF_INET6", text, rc);
}

/* A repeatable pseudo-random sequence. */
static unsigned long next_rand(unsigned long *x)
{
	*x = *x * 1103515245UL + 12345UL;
	return (*x >> 16) & 0xffff;
}
#endif
