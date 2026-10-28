/*
 * Names that differ only in case are different names: four files named
 * with the same letters in different cases coexist, each with its own data.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.4.9-filenames.001
 */
#include "pc.h"
#include <dirent.h>
#include <errno.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>

int main(void)
{
	static const char *names[] = { "Ab", "aB", "AB", "ab" };
	static const char *data[] = { "one", "two", "three", "four" };
	struct stat sb[4];
	char buf[16];
	int i, j, fd, seen[4] = { 0, 0, 0, 0 };
	DIR *d;
	struct dirent *e;
	ssize_t n;

	pc_begin("XBD.4.9-filenames.001");
	pc_make_dir("t");
	pc_test();
	for (i = 0; i < 4; i++) {
		char p[16];

		(void)strcpy(p, "t/");
		(void)strcat(p, names[i]);
		fd = open(p, O_WRONLY | O_CREAT | O_EXCL, 0600);
		pc_check(fd >= 0, "creating %s failed with %s after the names before it were created: it counts as the same name as one of them", p, pc_errname(errno));
		pc_check(write(fd, data[i], strlen(data[i])) == (ssize_t)strlen(data[i]), "write failed");
		(void)close(fd);
	}
	for (i = 0; i < 4; i++) {
		char p[16];

		(void)strcpy(p, "t/");
		(void)strcat(p, names[i]);
		pc_check(stat(p, &sb[i]) == 0, "stat(%s) failed: %s", p, pc_errname(errno));
		fd = open(p, O_RDONLY);
		pc_check(fd >= 0, "open(%s) failed: %s", p, pc_errname(errno));
		n = read(fd, buf, sizeof buf);
		(void)close(fd);
		pc_check(n == (ssize_t)strlen(data[i]) && memcmp(buf, data[i], (size_t)n) == 0, "%s does not hold its own data", p);
	}
	for (i = 0; i < 4; i++)
		for (j = i + 1; j < 4; j++)
			pc_check(sb[i].st_ino != sb[j].st_ino || sb[i].st_dev != sb[j].st_dev, "%s and %s are the same file", names[i], names[j]);
	d = opendir("t");
	pc_check(d != NULL, "opendir failed: %s", pc_errname(errno));
	while ((e = readdir(d)) != NULL)
		for (i = 0; i < 4; i++)
			if (strcmp(e->d_name, names[i]) == 0)
				seen[i]++;
	(void)closedir(d);
	for (i = 0; i < 4; i++)
		pc_check(seen[i] == 1, "the directory lists %s %d times, expected once", names[i], seen[i]);
	/* removing one leaves the others */
	pc_check(unlink("t/aB") == 0, "unlink failed");
	pc_check(stat("t/AB", &sb[0]) == 0 && stat("t/Ab", &sb[0]) == 0 && stat("t/ab", &sb[0]) == 0, "removing aB removed another name");
	pc_pass();
}
