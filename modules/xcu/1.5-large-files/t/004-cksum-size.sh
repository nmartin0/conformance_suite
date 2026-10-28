# cksum reports the exact size of a file larger than 4 GiB and the same checksum whether it reads the file or its content through a pipe from cat
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.1.5-large-files.004
. "$PC_CORE/pc.sh"
pc_begin XCU.1.5-large-files.004
LC_ALL=C; export LC_ALL
pc_top=$(pwd) || pc_unresolved "cannot find the working directory"
pc_wd=pcw.$$
mkdir "$pc_wd" && cd "$pc_wd" || pc_unresolved "cannot make a work directory"
trap 'cd "$pc_top" && rm -rf "$pc_wd"' EXIT
printf X | dd of=big bs=1 seek=4294967296 2>/dev/null || pc_notinuse "a file larger than 4 GiB cannot be created here"
[ -s big ] || pc_notinuse "a file larger than 4 GiB cannot be created here"

pc_test
set -- $(cksum big 2>ck.err) || pc_fail "cksum failed: $(cat ck.err)"
c1=$1; s1=$2
[ "$s1" = 4294967297 ] || pc_fail "cksum printed the size '$s1'"
set -- $(cat big | cksum) || pc_fail "cksum of the pipe failed"
[ "$2" = 4294967297 ] || pc_fail "cat | cksum printed the size '$2'"
[ "$1" = "$c1" ] || pc_fail "the checksums of the file and of the piped content differ"
pc_pass
