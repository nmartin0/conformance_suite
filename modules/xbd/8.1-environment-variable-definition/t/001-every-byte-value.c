/*
 * The value of an environment variable may be any sequence of bytes
 * except the null byte: a variable whose value holds each byte value from
 * 1 to 255, including the equals sign, reaches a new process image intact.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.8.1-environment-variable-definition.001
 */
#include "selfexec.h"

static void image(int fd)
{
	const char *v = getenv("PCV");
	char m[100];
	int i, n;

	if (v == NULL) {
		(void)write(fd, "PCV is not in the environment", 29);
		return;
	}
	if (strlen(v) != 255) {
		n = snprintf(m, sizeof m, "the value is %lu bytes long, not 255", (unsigned long)strlen(v));
		(void)write(fd, m, (size_t)n);
		return;
	}
	for (i = 0; i < 255; i++)
		if ((unsigned char)v[i] != (unsigned char)(i + 1)) {
			n = snprintf(m, sizeof m, "byte %d of the value is %d", i + 1, (unsigned char)v[i]);
			(void)write(fd, m, (size_t)n);
			return;
		}
	(void)write(fd, "ok", 2);
}

static int run_test(void)
{
	char entry[300], rep[300];
	char *envp[2];
	int i;

	pc_begin("XBD.8.1-environment-variable-definition.001");
	memcpy(entry, "PCV=", 4);
	for (i = 1; i < 256; i++)
		entry[3 + i] = (char)i;
	entry[259] = '\0';
	envp[0] = entry;
	envp[1] = NULL;
	pc_test();
	se_run(envp, rep, sizeof rep);
	pc_check(strcmp(rep, "ok") == 0, "the new image reports: %s", rep);
	pc_pass();
	return 0;
}

SE_MAIN
