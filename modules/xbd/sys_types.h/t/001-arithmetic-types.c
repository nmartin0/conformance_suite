/*
 * The sys/types.h types are arithmetic types.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.sys_types.h.001
 */
#include <sys/types.h>
#include "pc.h"

static blkcnt_t v_blkcnt_t = 3;
static blksize_t v_blksize_t = 3;
static clock_t v_clock_t = 3;
static clockid_t v_clockid_t = 3;
static dev_t v_dev_t = 3;
static fsblkcnt_t v_fsblkcnt_t = 3;
static fsfilcnt_t v_fsfilcnt_t = 3;
static gid_t v_gid_t = 3;
static id_t v_id_t = 3;
static ino_t v_ino_t = 3;
static mode_t v_mode_t = 3;
static nlink_t v_nlink_t = 3;
static off_t v_off_t = 3;
static pid_t v_pid_t = 3;
static size_t v_size_t = 3;
static ssize_t v_ssize_t = 3;
static time_t v_time_t = 3;
static uid_t v_uid_t = 3;

int main(void)
{
	pc_begin("XBD.sys_types.h.001");
	pc_test();
	v_blkcnt_t = v_blkcnt_t * 2 + 1;
	pc_check(v_blkcnt_t == 7 && v_blkcnt_t * 1.5 > 10.0, "blkcnt_t does not behave as an arithmetic type");
	v_blksize_t = v_blksize_t * 2 + 1;
	pc_check(v_blksize_t == 7 && v_blksize_t * 1.5 > 10.0, "blksize_t does not behave as an arithmetic type");
	v_clock_t = v_clock_t * 2 + 1;
	pc_check(v_clock_t == 7 && v_clock_t * 1.5 > 10.0, "clock_t does not behave as an arithmetic type");
	v_clockid_t = v_clockid_t * 2 + 1;
	pc_check(v_clockid_t == 7 && v_clockid_t * 1.5 > 10.0, "clockid_t does not behave as an arithmetic type");
	v_dev_t = v_dev_t * 2 + 1;
	pc_check(v_dev_t == 7 && v_dev_t * 1.5 > 10.0, "dev_t does not behave as an arithmetic type");
	v_fsblkcnt_t = v_fsblkcnt_t * 2 + 1;
	pc_check(v_fsblkcnt_t == 7 && v_fsblkcnt_t * 1.5 > 10.0, "fsblkcnt_t does not behave as an arithmetic type");
	v_fsfilcnt_t = v_fsfilcnt_t * 2 + 1;
	pc_check(v_fsfilcnt_t == 7 && v_fsfilcnt_t * 1.5 > 10.0, "fsfilcnt_t does not behave as an arithmetic type");
	v_gid_t = v_gid_t * 2 + 1;
	pc_check(v_gid_t == 7 && v_gid_t * 1.5 > 10.0, "gid_t does not behave as an arithmetic type");
	v_id_t = v_id_t * 2 + 1;
	pc_check(v_id_t == 7 && v_id_t * 1.5 > 10.0, "id_t does not behave as an arithmetic type");
	v_ino_t = v_ino_t * 2 + 1;
	pc_check(v_ino_t == 7 && v_ino_t * 1.5 > 10.0, "ino_t does not behave as an arithmetic type");
	v_mode_t = v_mode_t * 2 + 1;
	pc_check(v_mode_t == 7 && v_mode_t * 1.5 > 10.0, "mode_t does not behave as an arithmetic type");
	v_nlink_t = v_nlink_t * 2 + 1;
	pc_check(v_nlink_t == 7 && v_nlink_t * 1.5 > 10.0, "nlink_t does not behave as an arithmetic type");
	v_off_t = v_off_t * 2 + 1;
	pc_check(v_off_t == 7 && v_off_t * 1.5 > 10.0, "off_t does not behave as an arithmetic type");
	v_pid_t = v_pid_t * 2 + 1;
	pc_check(v_pid_t == 7 && v_pid_t * 1.5 > 10.0, "pid_t does not behave as an arithmetic type");
	v_size_t = v_size_t * 2 + 1;
	pc_check(v_size_t == 7 && v_size_t * 1.5 > 10.0, "size_t does not behave as an arithmetic type");
	v_ssize_t = v_ssize_t * 2 + 1;
	pc_check(v_ssize_t == 7 && v_ssize_t * 1.5 > 10.0, "ssize_t does not behave as an arithmetic type");
	v_time_t = v_time_t * 2 + 1;
	pc_check(v_time_t == 7 && v_time_t * 1.5 > 10.0, "time_t does not behave as an arithmetic type");
	v_uid_t = v_uid_t * 2 + 1;
	pc_check(v_uid_t == 7 && v_uid_t * 1.5 > 10.0, "uid_t does not behave as an arithmetic type");
	pc_pass();
}
