/*
 * A relative pathname of {PATH_MAX} - 1 bytes works; a longer one is refused.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.limits.h.019
 */
#include <errno.h>
#include <fcntl.h>
#include <limits.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>
#include "pc.h"

static char path[8192];
static char longer[8200];
static char msg[256];

/* Fills path with directory names, then a file name, strlen() == m. */
static void build(size_t m, size_t c)
{
	size_t rem = m, len, at = 0;

	while (rem > c) {
		len = rem - 2 < c ? rem - 2 : c;
		memset(path + at, 'd', len);
		at += len;
		path[at++] = '/';
		rem -= len + 1;
	}
	memset(path + at, 'f', rem);
	path[at + rem] = '\0';
}

/* Makes the parent directories and the file; returns an error text or NULL. */
static const char *make(void)
{
	char *s;
	int fd;

	for (s = strchr(path, '/'); s != NULL; s = strchr(s + 1, '/')) {
		*s = '\0';
		if (mkdir(path, 0700) != 0) {
			(void)snprintf(msg, sizeof msg, "mkdir() of a %zu-byte pathname failed with %s",
			    strlen(path), pc_errname(errno));
			*s = '/';
			return msg;
		}
		*s = '/';
	}
	fd = open(path, O_WRONLY | O_CREAT | O_EXCL, 0600);
	if (fd == -1) {
		(void)snprintf(msg, sizeof msg, "creating a file at a %zu-byte pathname failed with %s",
		    strlen(path), pc_errname(errno));
		return msg;
	}
	(void)close(fd);
	return NULL;
}

/* Removes what make() made. */
static void clean(void)
{
	char *s;

	(void)unlink(path);
	while ((s = strrchr(path, '/')) != NULL) {
		*s = '\0';
		(void)rmdir(path);
	}
}

static const char *attempt(size_t m, size_t c, int over)
{
	struct stat st;
	const char *err;
	int fd;

	build(m, c);
	if ((err = make()) == NULL) {
		if (stat(path, &st) != 0 || !S_ISREG(st.st_mode)) {
			(void)snprintf(msg, sizeof msg, "stat() of a %zu-byte pathname failed with %s",
			    m, pc_errname(errno));
			err = msg;
		} else if ((fd = open(path, O_RDONLY)) == -1) {
			(void)snprintf(msg, sizeof msg, "open() of a %zu-byte pathname failed with %s",
			    m, pc_errname(errno));
			err = msg;
		} else {
			(void)close(fd);
			if (over) {
				longer[0] = '.';
				longer[1] = '/';
				strcpy(longer + 2, path);
				if (stat(longer, &st) != -1 || errno != ENAMETOOLONG) {
					(void)snprintf(msg, sizeof msg,
					    "stat() of a %zu-byte pathname (limit %zu with the null) did not fail with ENAMETOOLONG",
					    m + 2, m + 1);
					err = msg;
				}
			}
		}
	}
	return err;
}

int main(void)
{
	long p, nm;
	size_t c, big;
	const char *err;

	pc_begin("XBD.limits.h.019");
	p = pathconf(".", _PC_PATH_MAX);
	nm = pathconf(".", _PC_NAME_MAX);
	c = nm == -1 || nm > 60 ? 60 : (size_t)nm;
	pc_test();
	pc_check(p == -1 || p >= 256, "pathconf(_PC_PATH_MAX) is %ld, below 256", p);

	/* The guaranteed minimum, 255 bytes and the null. */
	err = attempt(255, c, 0);
	clean();
	if (err != NULL)
		pc_fail("%s", err);

	if (p > 256) {
		big = (size_t)(p > 4096 ? 4096 : p) - 1;
		err = attempt(big, c, p <= 4096);
		clean();
		if (err != NULL)
			pc_fail("%s", err);
	}
	pc_pass();
}
