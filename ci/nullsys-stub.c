/*
 * nullsys-stub.c - the stub shell and the stub utilities of the negative
 * control (ci/nullsys.sh).
 *
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * One program, installed under many names. Run under the name NAME it
 *   1. appends "ID NAME" to $PC_NULLSYS_LOG (ID is $PC_ASSERTION), if both
 *      are set and the file can be written; failures are ignored;
 *   2. if NAME is "sh" and $PC_NULLSYS_REALSH is set (ci/nullsys.sh run
 *      --real-sh), puts $PC_NULLSYS_BIN in front of PATH and runs that
 *      shell with the same arguments instead;
 *   3. otherwise does nothing: exits 0 with no output, ignoring its
 *      arguments and input; or, when $PC_NULLSYS_FLIP is set and the
 *      program runs for a test ($PC_ASSERTION is set), prints "error" on
 *      standard output and exits 1. (The driver starts the shell under
 *      test once before any test, to check it; that run stays silent.)
 */
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

static void put(char **q, const char *s)
{
	size_t n = strlen(s);

	memcpy(*q, s, n);
	*q += n;
}

static void log_call(const char *name)
{
	const char *path = getenv("PC_NULLSYS_LOG");
	const char *id = getenv("PC_ASSERTION");
	char line[512], *q = line;
	int fd;

	if (path == NULL || id == NULL || *path == '\0' || *id == '\0' ||
	    strlen(id) + strlen(name) + 2 > sizeof line)
		return;
	put(&q, id);
	put(&q, " ");
	put(&q, name);
	put(&q, "\n");
	fd = open(path, O_WRONLY | O_APPEND | O_CREAT, 0666);
	if (fd >= 0) {
		(void)write(fd, line, (size_t)(q - line));
		(void)close(fd);
	}
}

int main(int argc, char **argv)
{
	const char *name = argv[0] != NULL ? argv[0] : "?";
	const char *slash = strrchr(name, '/');
	const char *realsh = getenv("PC_NULLSYS_REALSH");

	(void)argc;
	if (slash != NULL)
		name = slash + 1;
	log_call(name);
	if (strcmp(name, "sh") == 0 && realsh != NULL && *realsh != '\0') {
		const char *bin = getenv("PC_NULLSYS_BIN");
		const char *old = getenv("PATH");

		/* the driver's own checks (no assertion running) keep the real PATH */
		if (bin != NULL && *bin != '\0' && getenv("PC_ASSERTION") != NULL) {
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
		argv[0] = (char *)realsh;
		execv(realsh, argv);
		_exit(127);
	}
	if (getenv("PC_NULLSYS_FLIP") != NULL && getenv("PC_ASSERTION") != NULL) {
		(void)write(1, "error\n", 6);
		return 1;
	}
	return 0;
}
