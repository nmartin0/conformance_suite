/*
 * XDG_RUNTIME_DIR names a private directory of the user.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION CONV.xdg.001
 */
#include <sys/stat.h>
#include <unistd.h>
#include "pc.h"
#include "../../../../lib/cv.h"

int main(void)
{
	const char *d;
	struct stat st;

	pc_begin("CONV.xdg.001");
	d = cv_env("XDG_RUNTIME_DIR");
	if (d == NULL)
		pc_notinuse("XDG_RUNTIME_DIR is not set in the environment the suite was started from");
	pc_test();
	pc_check(d[0] == '/', "XDG_RUNTIME_DIR '%s' is not an absolute path", d);
	pc_check(stat(d, &st) == 0, "XDG_RUNTIME_DIR '%s' does not name an existing file", d);
	pc_check(S_ISDIR(st.st_mode), "XDG_RUNTIME_DIR '%s' is not a directory", d);
	pc_check(st.st_uid == geteuid(), "'%s' is owned by uid %ld, not the current user %ld",
	    d, (long)st.st_uid, (long)geteuid());
	pc_check((st.st_mode & 07777) == 0700, "'%s' has mode %04o, not 0700", d,
	    (unsigned)(st.st_mode & 07777));
	pc_pass();
}
