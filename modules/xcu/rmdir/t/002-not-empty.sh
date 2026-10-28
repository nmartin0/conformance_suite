# rmdir of a directory that is not empty fails with a diagnostic and removes nothing.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.rmdir.002
. "$PC_CORE/pc.sh"
pc_begin XCU.rmdir.002
mkdir d || pc_fail "cannot create d"
printf x >d/f || pc_fail "cannot create d/f"
pc_test
rmdir d >out 2>err </dev/null
rc=$?
[ "$rc" -ne 0 ] || pc_fail "rmdir exited 0 for a non-empty directory"
[ -s err ] || pc_fail "no diagnostic"
[ -f d/f ] || pc_fail "d/f is gone"
pc_pass
