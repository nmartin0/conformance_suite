/*
 * Protocols file: name, number, aliases.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION CONV.etcdb.013
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
	char *t = NULL, *p, *l, *f[64];
	int n, line = 0, entries = 0;
	unsigned long num;

	pc_begin("CONV.etcdb.013");
	t = cv_etc("PROTOCOLS", "/etc/protocols");
	pc_test();
	p = t;
	while ((l = cv_nextline(&p)) != NULL) {
		line++;
		cv_strip_comment(l);
		if (l[strspn(l, " \t")] == '\0')
			continue;
		entries++;
		n = cv_split(l, " \t", 1, f, 64);
		pc_check(n >= 2, "line %d lacks a protocol number", line);
		pc_check(cv_decimal32(f[1], &num) && num <= 255, "protocol number '%s' of '%s' (line %d) is not a number up to 255", f[1], f[0], line);
	}
	pc_check(entries > 0, "the protocols file has no entries");
	pc_pass();
}
