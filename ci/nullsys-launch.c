/*
 * nullsys-launch.c - starts a test of the negative control with the stub
 * library preloaded (ci/nullsys.sh).
 *
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * The compiler wrapper ci/nullsys.sh makes renames each linked test X to
 * X.real and puts this program in place of X. Run as X, it sets
 * LD_PRELOAD to $PC_NULLSYS_LIB, puts $PC_NULLSYS_BIN in front of PATH and
 * runs X.real with the same arguments. (pcexec passes PC_* variables to the
 * test and nothing else, which is why the library is named through one.)
 * LD_PRELOAD is not POSIX; the CI libraries that plant faults use it too
 * (RULES.md, rule 12).
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int main(int argc, char **argv)
{
	const char *lib = getenv("PC_NULLSYS_LIB");
	const char *bin = getenv("PC_NULLSYS_BIN");
	const char *old = getenv("PATH");
	size_t n;
	char *real;

	(void)argc;
	if (argv[0] == NULL)
		return 126;
	n = strlen(argv[0]);
	real = malloc(n + 6);
	if (real == NULL)
		return 126;
	strcpy(real, argv[0]);
	strcpy(real + n, ".real");
	if (lib != NULL && *lib != '\0')
		(void)setenv("LD_PRELOAD", lib, 1);
	if (bin != NULL && *bin != '\0') {
		char *np = malloc(strlen(bin) + (old ? strlen(old) : 0) + 2);

		if (np != NULL) {
			strcpy(np, bin);
			if (old != NULL && *old != '\0') {
				strcat(np, ":");
				strcat(np, old);
			}
			(void)setenv("PATH", np, 1);
		}
	}
	execv(real, argv);
	fprintf(stderr, "nullsys-launch: cannot run %s\n", real);
	return 127;
}
