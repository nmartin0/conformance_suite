/*
 * A new file's owner is the effective UID; its group is the directory's or the effective GID.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.open.048
 * MUTANT    open.creat_wrong_owner
 */
#include "pc.h"
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>

int main(void)
{
	struct stat d, f;
	int fd;

	pc_begin("XSH.open.048");
	pc_require_space();
	pc_check(stat(".", &d) == 0, "stat . failed");
	pc_test();
	fd = open("n", O_WRONLY | O_CREAT | O_EXCL, 0600);
	pc_expect_ok(fd, "open(\"n\", O_WRONLY|O_CREAT|O_EXCL)");
	pc_check(fstat(fd, &f) == 0, "fstat failed");
	pc_check(f.st_uid == geteuid(), "owner is %ld, effective UID is %ld",
	    (long)f.st_uid, (long)geteuid());
	pc_check(f.st_gid == getegid() || f.st_gid == d.st_gid,
	    "group is %ld: neither the effective GID %ld nor the directory's %ld",
	    (long)f.st_gid, (long)getegid(), (long)d.st_gid);
	pc_pass();
}
