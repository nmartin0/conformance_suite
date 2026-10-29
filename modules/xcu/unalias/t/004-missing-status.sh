# unalias of a missing name fails.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.unalias.004
. "$PC_CORE/pc.sh"
pc_begin XCU.unalias.004
pc_test
alias pcu4=x
unalias pcu4 >/dev/null 2>&1 || pc_unresolved "unalias of a defined alias fails, so its failure for a missing name proves nothing"
unalias pcnosuch3 >/dev/null 2>&1
rc=$?
[ "$rc" -gt 0 ] || pc_fail "unalias of a missing name exited $rc"
pc_pass
