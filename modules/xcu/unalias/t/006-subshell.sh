# unalias in a subshell leaves the parent's alias.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.unalias.006
. "$PC_CORE/pc.sh"
pc_begin XCU.unalias.006
alias pcu6='echo six'
pc_test
( unalias pcu6 )
alias pcu6 >/dev/null 2>&1 || pc_fail "alias removed in the parent by a subshell"
pc_pass
