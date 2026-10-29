# rmdir -p removes the named directory and then each parent in the path.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.rmdir.004
. "$PC_CORE/pc.sh"
pc_begin XCU.rmdir.004
mkdir -p a/b/c || pc_fail "cannot create a/b/c"
[ -d a/b/c ] || pc_fail "a/b/c was not created"
pc_test
rmdir -p a/b/c >out 2>err </dev/null
rc=$?
[ "$rc" -eq 0 ] || pc_fail "rmdir -p exited with status $rc"
[ ! -e a ] || pc_fail "a still exists"
pc_pass
