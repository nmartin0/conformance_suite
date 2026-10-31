# With -R and more than two operands, a target that does not exist is an error.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.cp.022
# MUTANT    cp.R_many_creates
# MUTANT    cp.R_many_status0
# MUTANT    cp.no_diag
# MUTANT    cp.diag_stdout
# VARIANT    cp.status2
# VARIANT    cp.status255
. "$PC_CORE/pc.sh"
pc_begin XCU.cp.022
mkdir d1 d2 || pc_fail "cannot create directories"
pc_test
cp -R d1 d2 newtarget >out 2>err </dev/null
rc=$?
[ "$rc" -ne 0 ] || pc_fail "cp -R exited 0 with three operands and a missing target"
[ -s err ] || pc_fail "no diagnostic"
pc_pass
