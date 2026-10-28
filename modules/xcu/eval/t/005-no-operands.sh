# eval with no operands, or only null operands, returns status zero even
# when the previous status was not zero.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.eval.005
. "$PC_CORE/pc.sh"
pc_begin XCU.eval.005
pc_test
false
eval
rc=$?
[ "$rc" -eq 0 ] || pc_fail "eval with no operand gave $rc"
(exit 5)
eval ""
rc=$?
[ "$rc" -eq 0 ] || pc_fail "eval \"\" gave $rc"
(exit 5)
eval "" ""
rc=$?
[ "$rc" -eq 0 ] || pc_fail "eval \"\" \"\" gave $rc"
pc_pass
