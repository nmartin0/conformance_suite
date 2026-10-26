/*
 * getenv() finds variables of the environment a process starts with,
 * matching whole names, case-sensitively.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.getenv.001
 * MUTANT    getenv.prefix_match
 * MUTANT    getenv.fold_case
 * MUTANT    getenv.empty_is_null
 */
#include "pc.h"
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

extern char **environ;

/* The new image: exit 0 if every lookup is right, else the failing check. */
static int child(void)
{
	const char *p;

	p = getenv("PCX_A");
	if (p == NULL || strcmp(p, "one") != 0)
		return 1;
	p = getenv("PCX_AB");
	if (p == NULL || strcmp(p, "two") != 0)
		return 2;
	p = getenv("PCX_E");
	if (p == NULL || *p != '\0')
		return 3;
	if (getenv("PCX_Z") != NULL)
		return 4;
	if (getenv("PCX_") != NULL)
		return 5;
	if (getenv("pcx_a") != NULL)
		return 6;
	return 0;
}

static const char *what[] = {
	"",
	"getenv(\"PCX_A\") is not \"one\"",
	"getenv(\"PCX_AB\") is not \"two\"",
	"getenv(\"PCX_E\") is not an empty string",
	"getenv(\"PCX_Z\") (not in the environment) is not a null pointer",
	"getenv(\"PCX_\") (a prefix of names in the environment) is not a null pointer",
	"getenv(\"pcx_a\") is not a null pointer (only PCX_A is in the environment)",
};

int main(int argc, char *argv[])
{
	char **envp;
	size_t n, i, k;
	int st;
	pid_t pid;

	if (argc == 2 && strcmp(argv[1], "child") == 0)
		_exit(child());
	pc_begin("XSH.getenv.001");
	pc_check(argc >= 1 && argv[0][0] == '/', "argv[0] is not an absolute path");
	pc_test();
	/* built now, so that the new image starts with PC_PHASE=test */
	for (n = 0; environ[n] != NULL; n++)
		;
	envp = malloc((n + 4) * sizeof *envp);
	if (envp == NULL)
		pc_unresolved("out of memory");
	/* PCX_AB comes before PCX_A, so that a prefix match finds the wrong one. */
	k = 0;
	envp[k++] = "PCX_AB=two";
	envp[k++] = "PCX_A=one";
	envp[k++] = "PCX_E=";
	for (i = 0; i < n; i++)
		if (strncmp(environ[i], "PCX_", 4) != 0 && strncmp(environ[i], "pcx_", 4) != 0)
			envp[k++] = environ[i];
	envp[k] = NULL;
	pid = fork();
	if (pid == -1)
		pc_unresolved("fork failed: %s", pc_errname(errno));
	if (pid == 0) {
		execve(argv[0], (char *[]){ argv[0], "child", NULL }, envp);
		_exit(99);
	}
	while (waitpid(pid, &st, 0) == -1)
		if (errno != EINTR)
			pc_unresolved("waitpid failed: %s", pc_errname(errno));
	if (!WIFEXITED(st) || WEXITSTATUS(st) == 99 || WEXITSTATUS(st) > 6)
		pc_unresolved("the exec child did not run (status %#x)", (unsigned)st);
	pc_check(WEXITSTATUS(st) == 0, "%s", what[WEXITSTATUS(st)]);
	pc_pass();
}
