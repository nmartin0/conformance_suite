# With -R and more than two operands, a target that does not exist is an error.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.cp.022
. "$PC_CORE/pc.sh"
pc_begin XCU.cp.022
mkdir d1 d2 || pc_fail "cannot create directories"
pc_test
cp -R d1 d2 newtarget >out 2>err </dev/null
rc=$?
[ "$rc" -ne 0 ] || pc_fail "cp -R exited 0 with three operands and a missing target"
pc_pass
