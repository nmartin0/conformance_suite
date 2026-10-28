/*
 * Uppercase and lowercase letters keep their identities in environment
 * variable names: variables whose names differ only in case stay distinct
 * in a new process image and through setenv() and getenv().
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.8.1-environment-variable-definition.002
 */
#include "selfexec.h"

static void image(int fd)
{
	const char *a = getenv("pcvar"), *b = getenv("PCVAR"), *c = getenv("PcVar");

	if (a == NULL || b == NULL || c == NULL)
		(void)write(fd, "one of the three variables is missing", 37);
	else if (strcmp(a, "lower") != 0 || strcmp(b, "UPPER") != 0 || strcmp(c, "Mixed") != 0)
		(void)write(fd, "the values were folded together", 31);
	else if (getenv("pcVAR") != NULL)
		(void)write(fd, "a variable that was never set exists", 36);
	else
		(void)write(fd, "ok", 2);
}

static int run_test(void)
{
	char rep[300];
	char *envp[4] = { (char *)"pcvar=lower", (char *)"PCVAR=UPPER", (char *)"PcVar=Mixed", NULL };

	pc_begin("XBD.8.1-environment-variable-definition.002");
	(void)unsetenv("pcset");
	(void)unsetenv("PCSET");
	if (setenv("pcset", "low", 1) != 0 || setenv("PCSET", "UP", 1) != 0)
		pc_unresolved("setenv failed");
	pc_test();
	pc_check(getenv("pcset") != NULL && strcmp(getenv("pcset"), "low") == 0, "getenv(\"pcset\") is not \"low\"");
	pc_check(getenv("PCSET") != NULL && strcmp(getenv("PCSET"), "UP") == 0, "getenv(\"PCSET\") is not \"UP\"");
	(void)unsetenv("PCSET");
	pc_check(getenv("pcset") != NULL && strcmp(getenv("pcset"), "low") == 0, "unsetting PCSET removed pcset");
	(void)unsetenv("pcset");
	se_run(envp, rep, sizeof rep);
	pc_check(strcmp(rep, "ok") == 0, "the new image reports: %s", rep);
	pc_pass();
	return 0;
}

SE_MAIN
