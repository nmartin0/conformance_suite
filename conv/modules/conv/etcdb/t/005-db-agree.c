/*
 * getpwnam and getgrnam agree with the files.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION CONV.etcdb.005
 */
#include <sys/stat.h>
#include <arpa/inet.h>
#include <ctype.h>
#include <errno.h>
#include <grp.h>
#include <netdb.h>
#include <pwd.h>
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
	char *pw, *gr, *p, *l, *f[16], *m, *mem;
	int n, checked = 0;
	struct passwd *pe;
	struct group *ge;
	unsigned long v;

	pc_begin("CONV.etcdb.005");
	if (cv_env("PASSWD") != NULL || cv_env("GROUP") != NULL)
		pc_notinuse("the files are overridden: the library reads the real ones");
	if (!cv_nss_files_first("passwd") || !cv_nss_files_first("group"))
		pc_notinuse("the name-service configuration lists another source ahead of the file");
	pw = cv_etc("PASSWD", "/etc/passwd");
	gr = cv_etc("GROUP", "/etc/group");
	pc_test();
	p = pw;
	while (checked < 300 && (l = cv_nextline(&p)) != NULL) {
		if (l[0] == '\0' || l[0] == '#' || l[0] == '+' || l[0] == '-')
			continue;
		n = cv_split(l, ":", 0, f, 16);
		if (n != 7 || !cv_decimal32(f[2], &v))
			continue;
		checked++;
		pe = getpwnam(f[0]);
		pc_check(pe != NULL, "getpwnam(\"%s\") finds nothing although the file lists it", f[0]);
		pc_check((unsigned long)pe->pw_uid == v, "getpwnam(\"%s\") gives uid %lu, the file %lu", f[0], (unsigned long)pe->pw_uid, v);
		pc_check(cv_decimal32(f[3], &v) && (unsigned long)pe->pw_gid == v, "getpwnam(\"%s\") gives gid %lu, the file %s", f[0], (unsigned long)pe->pw_gid, f[3]);
		pc_check(strcmp(pe->pw_dir, f[5]) == 0, "getpwnam(\"%s\") gives home '%s', the file '%s'", f[0], pe->pw_dir, f[5]);
		if (f[6][0] != '\0')
			pc_check(strcmp(pe->pw_shell, f[6]) == 0, "getpwnam(\"%s\") gives shell '%s', the file '%s'", f[0], pe->pw_shell, f[6]);
	}
	pc_check(checked > 0, "no password entry to compare");
	checked = 0;
	p = gr;
	while (checked < 300 && (l = cv_nextline(&p)) != NULL) {
		if (l[0] == '\0' || l[0] == '#' || l[0] == '+' || l[0] == '-')
			continue;
		n = cv_split(l, ":", 0, f, 16);
		if (n != 4 || !cv_decimal32(f[2], &v))
			continue;
		checked++;
		ge = getgrnam(f[0]);
		pc_check(ge != NULL, "getgrnam(\"%s\") finds nothing although the file lists it", f[0]);
		pc_check((unsigned long)ge->gr_gid == v, "getgrnam(\"%s\") gives gid %lu, the file %lu", f[0], (unsigned long)ge->gr_gid, v);
		/* every member the file names is among the group's members */
		mem = f[3];
		while (*mem != '\0') {
			size_t k = strcspn(mem, ","), i;
			int found = 0;

			for (i = 0; ge->gr_mem != NULL && ge->gr_mem[i] != NULL; i++) {
				m = ge->gr_mem[i];
				if (strlen(m) == k && strncmp(m, mem, k) == 0)
					found = 1;
			}
			pc_check(found, "getgrnam(\"%s\") does not list member %.*s named in the file", f[0], (int)k, mem);
			mem += k;
			if (*mem == ',')
				mem++;
		}
	}
	pc_pass();
}
