# A nonexistent operand gets a diagnostic and a non-zero status, and the other operands are still reported.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.du.009
. "$PC_CORE/pc.sh"
pc_begin XCU.du.009
mkdir d || pc_fail "cannot create d"
pc_test
du -s nonexistent d >out 2>err </dev/null
rc=$?
[ "$rc" -ne 0 ] || pc_fail "du exited 0 although an operand does not exist"
[ -s err ] || pc_fail "no diagnostic"
set -- $(cat out)
[ "$#" -eq 2 ] && [ "$2" = d ] || pc_fail "output is: $*"
pc_pass
