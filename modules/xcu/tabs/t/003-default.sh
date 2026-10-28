# tabs with no operand is tabs -8
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.tabs.003
#
. "$PC_CORE/pc.sh"
. "$PC_CORE/pcpty.sh"
pc_begin XCU.tabs.003
pc_pty_need
pc_pty_term
tb() { o=$1; shift; "$PC_PTY" -t 20 -o "$o" -- env "TERM=$PCT" tabs "$@"; }
tb eight -8 || pc_unresolved "tabs -8 is not possible on this terminal type"
pc_test
tb none || pc_fail "tabs without operands failed"
cmp -s eight none || pc_fail "tabs and tabs -8 produce different output"
pc_pass
