/*
 * The permission constants can be used singly or OR-ed as the third
 * argument of open() with no cast to mode_t: a file made that way gets
 * exactly those permission bits when the umask is zero.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.sys_stat.h.003
 */
#include "pc.h"
#include <errno.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>

/* Creates name with the given mode argument and a umask of 0; checks the permission bits. */
static void make(const char *name, int mode, unsigned want)
{
	struct stat st;
	int fd = open(name, O_WRONLY | O_CREAT | O_EXCL, mode);

	pc_check(fd != -1, "open(%s, O_CREAT, %#o) failed (%s)", name, (unsigned)mode, pc_errname(errno));
	close(fd);
	pc_check(stat(name, &st) == 0, "stat(%s) failed (%s)", name, pc_errname(errno));
	pc_check((st.st_mode & 07777) == want, "file made with mode argument %#o has permission bits %#o, expected %#o",
	    (unsigned)mode, (unsigned)(st.st_mode & 07777), want);
}

int main(void)
{
	pc_begin("XBD.sys_stat.h.003");
#if !defined(S_IRWXU) || !defined(S_IRUSR) || !defined(S_IWUSR) || !defined(S_IXUSR) || !defined(S_IRWXG) || !defined(S_IRGRP) || !defined(S_IWGRP) || !defined(S_IXGRP) || !defined(S_IRWXO) || !defined(S_IROTH) || !defined(S_IWOTH) || !defined(S_IXOTH)
	pc_unresolved("a permission constant is missing (the header check reports it)");
#else
	pc_require_space();
	(void)umask(0);
	pc_test();
	/* No cast to mode_t on any of these. */
	make("m1", S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH, 0644);
	make("m2", S_IRWXU | S_IRWXG | S_IRWXO, 0777);
	make("m3", S_IRUSR, 0400);
	make("m4", S_IWUSR | S_IXUSR | S_IXGRP | S_IWOTH, 0312);
	make("m5", S_IRWXU | S_IRGRP | S_IXGRP | S_IROTH | S_IXOTH, 0755);
	pc_pass();
#endif
}
