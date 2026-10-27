/*
 * In a directory holding only a regular file, a directory, a FIFO and a
 * symbolic link, no posix_getdents() entry has d_type DT_MQ, DT_SEM,
 * DT_SHM or DT_TMO, and each of the four has its own DT_ value or
 * DT_UNKNOWN.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.dirent.h.002
 */
#include "pc.h"
#include <dirent.h>
#include <errno.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

int main(void)
{
	pc_begin("XBD.dirent.h.002");
#if !defined(DT_MQ) || !defined(DT_SEM) || !defined(DT_SHM) || !defined(DT_REG) || !defined(DT_DIR) || !defined(DT_FIFO) || !defined(DT_LNK) || !defined(DT_UNKNOWN)
	pc_unresolved("a DT_ constant is missing (the header check reports it)");
#else
	char *buf = malloc(8192);
	int dfd, seen = 0;
	ssize_t n;

	if (buf == NULL)
		pc_unresolved("malloc failed");
	pc_make_file("f", "x");
	pc_make_dir("d");
	if (mkfifo("p", 0600) == -1)
		pc_unresolved("mkfifo failed (%s)", pc_errname(errno));
	pc_make_symlink("f", "l");
	dfd = open(".", O_RDONLY);
	if (dfd == -1)
		pc_unresolved("open of the directory failed (%s)", pc_errname(errno));
	pc_test();
	for (;;) {
		long off = 0;

		n = posix_getdents(dfd, buf, 8192, 0);
		pc_check(n >= 0, "posix_getdents failed (%s)", pc_errname(errno));
		if (n == 0)
			break;
		while (off < n) {
			struct posix_dent *d = (struct posix_dent *)(buf + off);
			int t = d->d_type;

			pc_check(d->d_reclen > 0, "d_reclen of an entry is zero");
			pc_check(t != DT_MQ && t != DT_SEM && t != DT_SHM,
			    "d_type of %s is DT_MQ, DT_SEM or DT_SHM, but the entry is no such object", d->d_name);
#ifdef DT_TMO
			pc_check(t != DT_TMO, "d_type of %s is DT_TMO, but the entry is no such object", d->d_name);
#endif
			if (!strcmp(d->d_name, "f")) {
				pc_check(t == DT_REG || t == DT_UNKNOWN, "d_type of a regular file is %d", t);
				seen |= 1;
			} else if (!strcmp(d->d_name, "d")) {
				pc_check(t == DT_DIR || t == DT_UNKNOWN, "d_type of a directory is %d", t);
				seen |= 2;
			} else if (!strcmp(d->d_name, "p")) {
				pc_check(t == DT_FIFO || t == DT_UNKNOWN, "d_type of a FIFO is %d", t);
				seen |= 4;
			} else if (!strcmp(d->d_name, "l")) {
				pc_check(t == DT_LNK || t == DT_UNKNOWN, "d_type of a symbolic link is %d", t);
				seen |= 8;
			}
			off += d->d_reclen;
		}
	}
	pc_check(seen == 15, "the directory listing missed one of the four files the test made (mask %d)", seen);
	close(dfd);
	free(buf);
	pc_pass();
#endif
}
