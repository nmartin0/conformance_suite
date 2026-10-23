/*
 * A pipe is owned by the effective user and group.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.pipe.004
 * MUTANT    pipe.wrong_owner
 */
#include "pc.h"
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>

int main(void)
{
	struct stat st;
	int p[2];

	pc_begin("XSH.pipe.004");
	pc_test();
	pc_expect_ok(pipe(p), "pipe()");
	pc_check(fstat(p[0], &st) == 0, "fstat failed");
	pc_check(st.st_uid == geteuid(), "owner %ld, effective UID %ld",
	    (long)st.st_uid, (long)geteuid());
	pc_check(st.st_gid == getegid(), "group %ld, effective GID %ld",
	    (long)st.st_gid, (long)getegid());
	pc_check(S_ISFIFO(st.st_mode), "fstat does not report a FIFO");
	pc_pass();
}
