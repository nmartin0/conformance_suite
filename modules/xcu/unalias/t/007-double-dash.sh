# unalias -- name removes the alias.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.unalias.007
. "$PC_CORE/pc.sh"
pc_begin XCU.unalias.007
alias pcu7='echo seven'
pc_test
unalias -- pcu7
rc=$?
[ "$rc" -eq 0 ] || pc_fail "unalias -- exited $rc"
if alias pcu7 >/dev/null 2>&1; then pc_fail "alias still defined"; fi
pc_pass
