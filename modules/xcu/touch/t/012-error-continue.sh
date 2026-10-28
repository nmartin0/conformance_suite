# If one operand cannot be touched, touch diagnoses it, handles the others, and exits non-zero.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.touch.012
. "$PC_CORE/pc.sh"
pc_begin XCU.touch.012
pc_test
touch nodir/x f >out 2>err </dev/null
rc=$?
[ "$rc" -ne 0 ] || pc_fail "touch exited 0 although nodir does not exist"
[ -s err ] || pc_fail "no diagnostic"
[ -f f ] || pc_fail "f was not created"
pc_pass
