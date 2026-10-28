/*
 * A string of the environment has the form name=value with the name
 * ending at the first equals sign: a value containing equals signs, an
 * empty value, and names of uppercase letters, digits and underscores
 * (not beginning with a digit) are all retained, and the new image's
 * environment holds exactly those strings.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.8.1-environment-variable-definition.003
 */
#include "selfexec.h"

static void image(int fd)
{
	const char *a = getenv("A_1"), *e = getenv("EMPTY"), *u = getenv("_U9");
	int n = 0;

	while (environ[n] != NULL)
		n++;
	if (n != 3)
		(void)write(fd, "the environment does not hold exactly three strings", 51);
	else if (a == NULL || strcmp(a, "x=y=z") != 0)
		(void)write(fd, "A_1 is not x=y=z", 16);
	else if (e == NULL || e[0] != '\0')
		(void)write(fd, "EMPTY is missing or not empty", 29);
	else if (u == NULL || strcmp(u, "v") != 0)
		(void)write(fd, "_U9 is not v", 12);
	else
		(void)write(fd, "ok", 2);
}

static int run_test(void)
{
	char rep[300];
	char *envp[4] = { (char *)"A_1=x=y=z", (char *)"EMPTY=", (char *)"_U9=v", NULL };

	pc_begin("XBD.8.1-environment-variable-definition.003");
	pc_test();
	se_run(envp, rep, sizeof rep);
	pc_check(strcmp(rep, "ok") == 0, "the new image reports: %s", rep);
	pc_pass();
	return 0;
}

SE_MAIN
