/*
 * The home directory of every account that can log in exists.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION CONV.layout.007
 */
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "pc.h"
#include "../../../../lib/cv.h"

/* is `shell` listed in the shells database text? */
static int listed(char *shells, const char *shell)
{
	char *copy = strdup(shells), *p = copy, *l;
	int found = 0;

	while (!found && (l = cv_nextline(&p)) != NULL) {
		cv_strip_comment(l);
		l += strspn(l, " \t");
		found = l[0] != '\0' && strcmp(l, shell) == 0;
	}
	free(copy);
	return found;
}

int main(void)
{
	const char *pf = cv_env("PASSWD"), *sf = cv_env("SHELLS");
	char *pw, *shells, *p, *l, *f[16];
	struct stat st;
	int checked = 0;

	pc_begin("CONV.layout.007");
	pw = cv_slurp(pf != NULL ? pf : "/etc/passwd", NULL);
	if (pw == NULL)
		pc_notinuse("there is no password file in /etc (another database is in use)");
	shells = cv_slurp(sf != NULL ? sf : "/etc/shells", NULL);
	if (shells == NULL)
		pc_notinuse("there is no shells file, so the accounts that can log in cannot be told apart");
	pc_test();
	p = pw;
	while ((l = cv_nextline(&p)) != NULL) {
		if (l[0] == '\0' || l[0] == '#' || l[0] == '+' || l[0] == '-' || cv_split(l, ":", 0, f, 16) != 7)
			continue;
		if (!listed(shells, f[6]))
			continue;	/* an account that is not a login account */
		checked++;
		pc_check(f[5][0] == '/', "home of '%s' is '%s', not an absolute path", f[0], f[5]);
		pc_check(stat(f[5], &st) == 0 && S_ISDIR(st.st_mode),
		    "home directory %s of login account '%s' does not exist", f[5], f[0]);
	}
	if (checked == 0)
		pc_notinuse("no account has a shell listed in the shells file");
	pc_pass();
}
