/*
 * getservbyname agrees with the services file.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION CONV.etcdb.012
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

#define MAXE 4000
static char sname[MAXE][48], sproto[MAXE][16];
static unsigned long sport[MAXE];
static char salias[MAXE][160];	/* aliases, blank separated, each surrounded by blanks */

/* is name one of entry e's aliases? */
static int inlist(int e, const char *name)
{
	char key[64];

	(void)snprintf(key, sizeof key, " %s ", name);
	return strstr(salias[e], key) != NULL;
}

int main(void)
{
	char *t = NULL, *p, *l, *f[64], *slash;
	int n, entries = 0, i, j, dup, checked = 0;
	unsigned long port;
	struct servent *se;

	pc_begin("CONV.etcdb.012");
	if (cv_env("SERVICES") != NULL)
		pc_notinuse("the services file is overridden: the library reads the real one");
	if (!cv_nss_files_first("services"))
		pc_notinuse("the name-service configuration lists another source ahead of the file");
	t = cv_etc("SERVICES", "/etc/services");
	pc_test();
	p = t;
	while (entries < MAXE && (l = cv_nextline(&p)) != NULL) {
		cv_strip_comment(l);
		n = cv_split(l, " \t", 1, f, 64);
		if (n < 2 || (slash = strchr(f[1], '/')) == NULL)
			continue;
		*slash = '\0';
		if (!cv_decimal32(f[1], &port) || port > 65535 || strlen(f[0]) >= sizeof sname[0] || strlen(slash + 1) >= sizeof sproto[0])
			continue;
		(void)strcpy(sname[entries], f[0]);
		(void)strcpy(sproto[entries], slash + 1);
		salias[entries][0] = ' ';
		salias[entries][1] = '\0';
		for (i = 2; i < n; i++)
			if (strlen(salias[entries]) + strlen(f[i]) + 2 < sizeof salias[0]) {
				(void)strcat(salias[entries], f[i]);
				(void)strcat(salias[entries], " ");
			}
		sport[entries++] = port;
	}
	for (i = 0; i < entries && checked < 400; i++) {
		if (strcmp(sproto[i], "tcp") != 0 && strcmp(sproto[i], "udp") != 0)
			continue;
		dup = 0;
		for (j = 0; j < entries; j++)
			if (j != i && strcmp(sproto[j], sproto[i]) == 0 && (strcmp(sname[j], sname[i]) == 0 || inlist(j, sname[i])))
				dup = 1;
		for (j = 0; j < entries; j++)
			if (j != i && strcmp(sproto[j], sproto[i]) == 0 && inlist(i, sname[j]))
				dup = 1;
		if (dup)
			continue;	/* the file itself is ambiguous for this name */
		checked++;
		se = getservbyname(sname[i], sproto[i]);
		pc_check(se != NULL, "getservbyname(\"%s\", \"%s\") finds nothing although the file lists it", sname[i], sproto[i]);
		pc_check((unsigned long)ntohs((uint16_t)se->s_port) == sport[i], "getservbyname(\"%s\", \"%s\") gives port %u, the file %lu",
		    sname[i], sproto[i], (unsigned)ntohs((uint16_t)se->s_port), sport[i]);
	}
	if (checked == 0)
		pc_notinuse("the services file has no tcp or udp entry to compare");
	pc_pass();
}
