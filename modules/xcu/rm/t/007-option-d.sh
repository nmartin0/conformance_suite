# rm -d removes an empty directory and a plain file, but fails on a directory that is not empty.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.rm.007
. "$PC_CORE/pc.sh"
pc_begin XCU.rm.007
mkdir e n || pc_fail "cannot create directories"
printf x >n/f || pc_fail "cannot create n/f"
printf x >plain || pc_fail "cannot create plain"
pc_test
rm -d e plain >out 2>err </dev/null
rc=$?
[ "$rc" -eq 0 ] || pc_fail "rm -d exited with status $rc"
[ ! -e e ] || pc_fail "empty directory was not removed"
[ ! -e plain ] || pc_fail "plain file was not removed"
rm -d n >out 2>err </dev/null
rc=$?
[ "$rc" -ne 0 ] || pc_fail "rm -d exited 0 for a directory that is not empty"
[ -f n/f ] || pc_fail "n/f was removed"
pc_pass
