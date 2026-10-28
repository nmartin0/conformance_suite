/*
 * [XSI] In an NLSPATH template %% stands for one percent sign, and %t and
 * %c are replaced by nothing when the locale name has no territory or
 * codeset element (the POSIX locale), so catopen() finds the catalog at
 * the pathname the expansion gives.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.8.2-internationalization-variables.005
 */
#include "pc.h"
#include <errno.h>
#include <nl_types.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define BAD ((nl_catd)-1)

static void make(const char *dir, const char *cat)
{
	char cmd[400], path[300];

	pc_make_dir(dir);
	(void)snprintf(path, sizeof path, "%s/%s", dir, cat);
	pc_make_file("pc005.msg", "$set 1 first set\n1 Hello catalog\n");
	(void)snprintf(cmd, sizeof cmd, "gencat '%s' pc005.msg >/dev/null 2>&1", path);
	if (system(cmd) != 0)
		pc_unresolved("the gencat utility could not build the test catalog");
}

static void expect_found(const char *tmpl, const char *what)
{
	nl_catd cd;
	const char *m;

	if (setenv("NLSPATH", tmpl, 1) != 0)
		pc_unresolved("setenv NLSPATH failed");
	errno = 0;
	cd = catopen("pc5cat", NL_CAT_LOCALE);
	if (cd == BAD) {
		pc_fail("catopen did not find the catalog with NLSPATH=%s (%s): %s", tmpl, what, pc_errname(errno));
		return;
	}
	m = catgets(cd, 1, 1, "default");
	pc_check(strcmp(m, "Hello catalog") == 0, "NLSPATH=%s (%s): the message is \"%s\"", tmpl, what, m);
	(void)catclose(cd);
}

int main(void)
{
	pc_begin("XBD.8.2-internationalization-variables.005");
	if (getenv("NLSPATH") != NULL && geteuid() == 0)
		pc_unresolved("NLSPATH was in the environment at start and the process is privileged: catopen() is undefined");
	(void)unsetenv("LC_ALL");
	(void)unsetenv("LC_MESSAGES");
	(void)unsetenv("LANG");
	make("pc%dir", "pc5cat");
	make("tcdir", "pc5cat");
	pc_test();
	expect_found("pc%%dir/%N", "%% is one percent sign");
	expect_found("tc%tdir/%N", "%t is empty");
	expect_found("tc%cdir/%N", "%c is empty");
	expect_found("t%t%cc%tdir/%N", "%t and %c are empty, side by side");
	pc_pass();
}
