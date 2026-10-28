# shift without an operand moves each positional parameter down by one,
# drops the first, and lowers $# by one.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.shift.001
. "$PC_CORE/pc.sh"
pc_begin XCU.shift.001
pc_test
set a b c d
shift
rc=$?
[ "$rc" -eq 0 ] || pc_fail "shift gave status $rc"
[ "$#" -eq 3 ] || pc_fail "\$# is $#, expected 3"
[ "$1$2$3" = bcd ] || pc_fail "parameters are '$*', expected b c d"
[ -z "${4+set}" ] || pc_fail "parameter 4 is still set"
[ "$*" = "b c d" ] || pc_fail "\$* is '$*'"
pc_pass
