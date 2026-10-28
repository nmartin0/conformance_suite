# With -f, nonexistent operands and an empty operand list give no diagnostic and status 0; existing operands are still removed.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.rm.003
. "$PC_CORE/pc.sh"
pc_begin XCU.rm.003
printf a >a || pc_fail "cannot create a"
pc_test
rm -f nonexistent >out 2>err </dev/null
rc=$?
[ "$rc" -eq 0 ] || pc_fail "rm -f nonexistent exited with status $rc"
[ ! -s err ] || pc_fail "diagnostic written for a nonexistent operand"
rm -f >out 2>err </dev/null
rc=$?
[ "$rc" -eq 0 ] || pc_fail "rm -f with no operand exited with status $rc"
[ ! -s err ] || pc_fail "diagnostic written for no operands"
rm -f nonexistent a >out 2>err </dev/null
rc=$?
[ "$rc" -eq 0 ] || pc_fail "rm -f nonexistent a exited with status $rc"
[ ! -e a ] || pc_fail "a was not removed"
pc_pass
