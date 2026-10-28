# unalias name removes the alias.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.unalias.001
. "$PC_CORE/pc.sh"
pc_begin XCU.unalias.001
alias pcu1='echo aliased'
pc_test
unalias pcu1
rc=$?
[ "$rc" -eq 0 ] || pc_fail "unalias exited $rc"
if alias pcu1 >/dev/null 2>&1; then
	pc_fail "alias still defined after unalias"
fi
got=$(eval pcu1 2>/dev/null)
[ "$got" != aliased ] || pc_fail "removed alias is still substituted"
pc_pass
