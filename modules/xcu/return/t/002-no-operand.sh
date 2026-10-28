# return with no operand uses the status of the last command executed.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.return.002
. "$PC_CORE/pc.sh"
pc_begin XCU.return.002
pc_test
pcf() { false; return; }
pcf
rc=$?
[ "$rc" -eq 1 ] || pc_fail "after false gave $rc"
pcf() { (exit 4); return; }
pcf
rc=$?
[ "$rc" -eq 4 ] || pc_fail "after (exit 4) gave $rc"
(exit 9)
pcf() { true; return; }
pcf
rc=$?
[ "$rc" -eq 0 ] || pc_fail "after true gave $rc"
pc_pass
