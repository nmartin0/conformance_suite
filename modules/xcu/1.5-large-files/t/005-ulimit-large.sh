# ulimit -f accepts a value above 2^32 and reports the same value back, and with a limit just above the size of a file of 4294967297 bytes, a write that produces such a file succeeds
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.1.5-large-files.005
. "$PC_CORE/pc.sh"
pc_begin XCU.1.5-large-files.005
LC_ALL=C; export LC_ALL
pc_top=$(pwd) || pc_unresolved "cannot find the working directory"
pc_wd=pcw.$$
mkdir "$pc_wd" && cd "$pc_wd" || pc_unresolved "cannot make a work directory"
trap 'cd "$pc_top" && rm -rf "$pc_wd"' EXIT
cur=$(ulimit -f 2>/dev/null) || pc_notinuse "ulimit -f is not available"
case $cur in
[0-9]*) [ "$cur" -gt 5000000000 ] 2>/dev/null || pc_notinuse "the current limit cannot be raised or lowered to the test value" ;;
esac
pc_test
r=$(ulimit -f 5000000000 2>/dev/null && ulimit -f 2>/dev/null) || pc_fail "ulimit -f 5000000000 failed"
[ "$r" = 5000000000 ] || pc_fail "ulimit -f reported '$r'"
(
	ulimit -f 8388610 2>/dev/null || exit 3
	printf X | dd of=big3 bs=1 seek=4294967296 2>/dev/null
) 
rc=$?
[ "$rc" != 3 ] || pc_fail "ulimit -f 8388610 failed"
[ -s big3 ] || pc_notinuse "a file larger than 4 GiB cannot be created here"
[ "$rc" = 0 ] || pc_fail "the write under a limit of 8388610 blocks failed with status $rc"
pc_pass
