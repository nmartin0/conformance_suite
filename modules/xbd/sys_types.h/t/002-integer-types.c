/*
 * Integer, signed and unsigned classification of sys/types.h types.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.sys_types.h.002
 */
#include <sys/types.h>
#include "pc.h"

_Static_assert((blkcnt_t)7 % (blkcnt_t)3 == 1, "blkcnt_t is not an integer type");
_Static_assert((blksize_t)7 % (blksize_t)3 == 1, "blksize_t is not an integer type");
_Static_assert((dev_t)7 % (dev_t)3 == 1, "dev_t is not an integer type");
_Static_assert((fsblkcnt_t)7 % (fsblkcnt_t)3 == 1, "fsblkcnt_t is not an integer type");
_Static_assert((fsfilcnt_t)7 % (fsfilcnt_t)3 == 1, "fsfilcnt_t is not an integer type");
_Static_assert((gid_t)7 % (gid_t)3 == 1, "gid_t is not an integer type");
_Static_assert((id_t)7 % (id_t)3 == 1, "id_t is not an integer type");
_Static_assert((ino_t)7 % (ino_t)3 == 1, "ino_t is not an integer type");
_Static_assert((mode_t)7 % (mode_t)3 == 1, "mode_t is not an integer type");
_Static_assert((nlink_t)7 % (nlink_t)3 == 1, "nlink_t is not an integer type");
_Static_assert((off_t)7 % (off_t)3 == 1, "off_t is not an integer type");
_Static_assert((pid_t)7 % (pid_t)3 == 1, "pid_t is not an integer type");
_Static_assert((size_t)7 % (size_t)3 == 1, "size_t is not an integer type");
_Static_assert((ssize_t)7 % (ssize_t)3 == 1, "ssize_t is not an integer type");
_Static_assert((uid_t)7 % (uid_t)3 == 1, "uid_t is not an integer type");
_Static_assert((blkcnt_t)-1 < 0, "blkcnt_t is not a signed integer type");
_Static_assert((off_t)-1 < 0, "off_t is not a signed integer type");
_Static_assert((blksize_t)-1 < 0, "blksize_t is not a signed integer type");
_Static_assert((pid_t)-1 < 0, "pid_t is not a signed integer type");
_Static_assert((ssize_t)-1 < 0, "ssize_t is not a signed integer type");
_Static_assert((fsblkcnt_t)-1 > 0, "fsblkcnt_t is not an unsigned integer type");
_Static_assert((fsfilcnt_t)-1 > 0, "fsfilcnt_t is not an unsigned integer type");
_Static_assert((ino_t)-1 > 0, "ino_t is not an unsigned integer type");
_Static_assert((size_t)-1 > 0, "size_t is not an unsigned integer type");

int main(void)
{
	pc_begin("XBD.sys_types.h.002");
	pc_test();
	pc_pass();
}
