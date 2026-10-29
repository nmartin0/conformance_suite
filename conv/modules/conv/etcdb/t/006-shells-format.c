/*
 * Shells file: one absolute path per line, each an executable file.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION CONV.etcdb.006
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
	char *t = NULL, *p, *l;
	struct stat st;
	int line = 0, entries = 0;

	pc_begin("CONV.etcdb.006");
	t = cv_etc("SHELLS", "/etc/shells");
	pc_test();
	p = t;
	while ((l = cv_nextline(&p)) != NULL) {
		line++;
		cv_strip_comment(l);
		l += strspn(l, " \t");
		if (l[0] == '\0')
			continue;
		entries++;
		pc_check(l[0] == '/', "shells entry '%s' (line %d) is not an absolute path", l, line);
		pc_check(strpbrk(l, " \t") == NULL, "shells entry '%s' (line %d) has white space inside", l, line);
		pc_check(stat(l, &st) == 0 && S_ISREG(st.st_mode) && access(l, X_OK) == 0,
		    "listed shell %s (line %d) is not an executable file", l, line);
	}
	pc_check(entries > 0, "the shells file lists no shell");
	pc_pass();
}
