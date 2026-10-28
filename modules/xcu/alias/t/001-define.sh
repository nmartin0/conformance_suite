# alias name=string defines an alias that is then substituted.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.alias.001
. "$PC_CORE/pc.sh"
pc_begin XCU.alias.001
pc_test
alias pcal1='echo hello'
rc=$?
[ "$rc" -eq 0 ] || pc_fail "alias definition exited $rc"
got=$(eval pcal1)
[ "$got" = hello ] || pc_fail "alias not substituted, got '$got'"
pc_pass
