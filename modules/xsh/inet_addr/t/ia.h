/*
 * ia.h - helpers shared by the inet_addr() and inet_ntoa() tests.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 */
#ifndef IA_H
#define IA_H
#include "pc.h"
#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <string.h>

struct row {
	const char *text;
	unsigned char b[4];	/* the bytes the address holds, left to right */
};

/* The four bytes of an in_addr_t value as they sit in memory. */
static void bytes_of(in_addr_t v, unsigned char out[4])
{
	memcpy(out, &v, 4);
}

static void run_rows(const struct row *rows, int n)
{
	int i;

	for (i = 0; i < n; i++) {
		in_addr_t v = inet_addr(rows[i].text);
		unsigned char b[4];

		bytes_of(v, b);
		pc_check(memcmp(b, rows[i].b, 4) == 0, "inet_addr(\"%s\") holds bytes %u.%u.%u.%u, expected %u.%u.%u.%u",
		    rows[i].text, b[0], b[1], b[2], b[3], rows[i].b[0], rows[i].b[1], rows[i].b[2], rows[i].b[3]);
	}
}
#endif
