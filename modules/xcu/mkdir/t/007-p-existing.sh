# With -p, an operand that names an existing directory is ignored without error or diagnostic.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.mkdir.007
. "$PC_CORE/pc.sh"
pc_begin XCU.mkdir.007
mkdir d || pc_fail "cannot create d"
pc_test
mkdir -p d >out 2>err </dev/null
rc=$?
[ "$rc" -eq 0 ] || pc_fail "mkdir -p d exited with status $rc"
mkdir -p d/e >out 2>err </dev/null
rc=$?
[ "$rc" -eq 0 ] || pc_fail "mkdir -p d/e exited with status $rc"
[ -d d/e ] || pc_fail "d/e was not created"
[ ! -s err ] || pc_fail "diagnostic written"
pc_pass
