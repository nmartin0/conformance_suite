# A nonexistent operand without -f gets a diagnostic and a non-zero status, and the other operands are still removed.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.rm.002
# MUTANT    rm.nonexist_silent
# MUTANT    rm.nonexist_status0
# MUTANT    rm.nonexist_aborts
# MUTANT    rm.nonexist_diag_stdout
# VARIANT   rm.status2
# VARIANT   rm.status255
# VARIANT   rm.ref
. "$PC_CORE/pc.sh"
pc_begin XCU.rm.002
printf a >a || pc_fail "cannot create a"
printf b >b || pc_fail "cannot create b"
pc_test
rm a nonexistent b >out 2>err </dev/null
rc=$?
[ "$rc" -ne 0 ] || pc_fail "rm exited 0 although an operand does not exist"
[ -s err ] || pc_fail "no diagnostic"
[ ! -e a ] && [ ! -e b ] || pc_fail "the existing operands were not removed"
pc_pass
