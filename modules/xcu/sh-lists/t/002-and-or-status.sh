# The status of an AND or OR list is the status of the last command that was
# executed.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.sh-lists.002
. "$PC_CORE/pc.sh"
pc_begin XCU.sh-lists.002
pc_test
true && false
[ "$?" -eq 1 ] || pc_fail "true && false"
false && true
[ "$?" -eq 1 ] || pc_fail "false && true"
false || true
[ "$?" -eq 0 ] || pc_fail "false || true"
true || false
[ "$?" -eq 0 ] || pc_fail "true || false"
(exit 3) || (exit 4)
[ "$?" -eq 4 ] || pc_fail "(exit 3) || (exit 4)"
(exit 3) && (exit 4)
[ "$?" -eq 3 ] || pc_fail "(exit 3) && (exit 4)"
true && (exit 5)
[ "$?" -eq 5 ] || pc_fail "true && (exit 5)"
pc_pass
