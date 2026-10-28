/*
 * Every character of the portable filename character set can be used in a
 * filename: single-character names (except dot, which names a directory),
 * a name made of all of them, and a name 14 bytes long.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.4.10-filename-portability.001
 */
#include "pc.h"
#include <dirent.h>
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>

static const char set[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789._-";

static int listed(const char *dir, const char *name)
{
	DIR *d = opendir(dir);
	struct dirent *e;
	int n = 0;

	if (d == NULL)
		return -1;
	while ((e = readdir(d)) != NULL)
		if (strcmp(e->d_name, name) == 0)
			n++;
	(void)closedir(d);
	return n;
}

static void use_name(const char *name)
{
	char p[300];
	int fd;
	struct stat sb;

	(void)snprintf(p, sizeof p, "t/%s", name);
	fd = open(p, O_WRONLY | O_CREAT | O_EXCL, 0600);
	pc_check(fd >= 0, "creating a file named \"%s\" failed: %s", name, pc_errname(errno));
	pc_check(write(fd, name, strlen(name)) == (ssize_t)strlen(name), "write to \"%s\" failed", name);
	(void)close(fd);
	pc_check(stat(p, &sb) == 0 && sb.st_size == (off_t)strlen(name), "\"%s\" cannot be found after it was created", name);
	pc_check(listed("t", name) == 1, "the directory does not list \"%s\" exactly once", name);
	pc_check(rename(p, "t/.r") == 0 && rename("t/.r", p) == 0, "rename of \"%s\" failed: %s", name, pc_errname(errno));
	pc_check(unlink(p) == 0, "unlink of \"%s\" failed: %s", name, pc_errname(errno));
}

int main(void)
{
	size_t i;
	char one[2], mid[16];
	long maxn;

	pc_begin("XBD.4.10-filename-portability.001");
	pc_make_dir("t");
	maxn = pathconf("t", _PC_NAME_MAX);
	pc_test();
	for (i = 0; i < sizeof set - 1; i++) {
		if (set[i] == '.')
			continue;	/* a lone dot names the directory itself */
		one[0] = set[i];
		one[1] = '\0';
		use_name(one);
	}
	if (maxn < 0 || maxn >= (long)sizeof set - 1)
		use_name(set);	/* all the characters in one name */
	(void)strcpy(mid, "a.b_c-d.e_f-gh");	/* 14 bytes */
	use_name(mid);
	use_name("-lead-hyphen");
	use_name(".hidden");
	use_name("..two");
	use_name("a.");
	pc_pass();
}
