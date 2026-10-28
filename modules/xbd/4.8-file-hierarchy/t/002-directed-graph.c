/*
 * One file can have several directory entries, in one directory or in
 * several: they share the file, and removing one name keeps the file
 * reachable by the others.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.4.8-file-hierarchy.002
 */
#include "pc.h"
#include <errno.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>

static int same(const struct stat *a, const struct stat *b)
{
	return a->st_dev == b->st_dev && a->st_ino == b->st_ino;
}

int main(void)
{
	struct stat a, b, c;
	char buf[16];
	int fd;
	ssize_t n;

	pc_begin("XBD.4.8-file-hierarchy.002");
	pc_make_file("one", "first");
	pc_make_dir("d1");
	pc_make_dir("d2");
	pc_test();
	pc_check(link("one", "two") == 0, "link() failed: %s", pc_errname(errno));
	pc_check(link("one", "d1/three") == 0, "link() into a directory failed: %s", pc_errname(errno));
	pc_check(link("d1/three", "d2/four") == 0, "link() from a name in another directory failed: %s", pc_errname(errno));
	pc_check(stat("one", &a) == 0 && stat("two", &b) == 0 && stat("d1/three", &c) == 0, "stat failed");
	pc_check(same(&a, &b) && same(&a, &c), "the names do not refer to the same file");
	pc_check(stat("d2/four", &b) == 0 && same(&a, &b), "d2/four does not refer to the same file");
	pc_check(a.st_nlink == 4, "st_nlink is %ld with four names, expected 4", (long)a.st_nlink);
	/* writing through one name shows through the others */
	fd = open("d1/three", O_WRONLY | O_APPEND);
	pc_check(fd >= 0, "open failed: %s", pc_errname(errno));
	pc_check(write(fd, "+more", 5) == 5, "write failed");
	(void)close(fd);
	fd = open("d2/four", O_RDONLY);
	pc_check(fd >= 0, "open failed: %s", pc_errname(errno));
	n = read(fd, buf, sizeof buf);
	(void)close(fd);
	pc_check(n == 10 && memcmp(buf, "first+more", 10) == 0, "data written through one name is not seen through another");
	/* removing names one at a time leaves the file reachable until the last goes */
	pc_check(unlink("one") == 0 && unlink("two") == 0, "unlink failed: %s", pc_errname(errno));
	pc_check(stat("d1/three", &b) == 0 && same(&a, &b), "the file is no longer reachable by its remaining names");
	pc_check(b.st_nlink == 2, "st_nlink is %ld with two names left, expected 2", (long)b.st_nlink);
	pc_check(unlink("d1/three") == 0 && stat("d2/four", &b) == 0 && b.st_nlink == 1, "after removing three names the last one does not show st_nlink 1");
	pc_check(unlink("d2/four") == 0 && stat("d2/four", &b) == -1 && errno == ENOENT, "the last name was not removed");
	pc_pass();
}
