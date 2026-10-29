/*
 * Protocol numbers: tcp 6, udp 17, icmp 1, in the file and through getprotobyname.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION CONV.etcdb.014
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

static const struct { const char *name; unsigned long num; } want[] = {
	{ "icmp", 1 }, { "tcp", 6 }, { "udp", 17 }
};

int main(void)
{
	char *t = NULL, *p, *l, *f[64];
	int n, i, k;
	unsigned long num;
	int seen[3] = { 0, 0, 0 };
	struct protoent *pe;

	pc_begin("CONV.etcdb.014");
	t = cv_etc("PROTOCOLS", "/etc/protocols");
	pc_test();
	p = t;
	while ((l = cv_nextline(&p)) != NULL) {
		cv_strip_comment(l);
		n = cv_split(l, " \t", 1, f, 64);
		if (n < 2 || !cv_decimal32(f[1], &num))
			continue;
		for (k = 0; k < 3; k++)
			for (i = 0; i < n; i++)
				if (i != 1 && strcmp(f[i], want[k].name) == 0) {
					seen[k] = 1;
					pc_check(num == want[k].num, "'%s' is protocol %lu in the file, not %lu", want[k].name, num, want[k].num);
				}
	}
	for (k = 0; k < 3; k++)
		pc_check(seen[k], "the protocols file has no entry for %s", want[k].name);
	if (cv_env("PROTOCOLS") == NULL && cv_nss_files_first("protocols"))
		for (k = 0; k < 3; k++) {
			pe = getprotobyname(want[k].name);
			pc_check(pe != NULL, "getprotobyname(\"%s\") finds nothing", want[k].name);
			pc_check((unsigned long)pe->p_proto == want[k].num, "getprotobyname(\"%s\") gives %d, not %lu", want[k].name, pe->p_proto, want[k].num);
		}
	pc_pass();
}
