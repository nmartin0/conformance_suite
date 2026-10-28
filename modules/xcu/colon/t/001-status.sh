# The null utility exits with status zero, with or without operands and
# whatever the status before it was.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.colon.001
. "$PC_CORE/pc.sh"
pc_begin XCU.colon.001
pc_test
:
rc=$?
[ "$rc" -eq 0 ] || pc_fail "colon exited with status $rc"
: a b c
rc=$?
[ "$rc" -eq 0 ] || pc_fail "colon with operands exited with status $rc"
false
:
rc=$?
[ "$rc" -eq 0 ] || pc_fail "colon after a failing command exited with status $rc"
pc_pass
