# tab list notations are interchangeable
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.tabs.004
#
. "$PC_CORE/pc.sh"
. "$PC_CORE/pcpty.sh"
pc_begin XCU.tabs.004
pc_pty_need
pc_pty_term
tb() { o=$1; shift; "$PC_PTY" -t 20 -o "$o" -- env "TERM=$PCT" tabs "$@"; }
tb a 1,10,20,30 || pc_unresolved "setting explicit tab stops is not possible on this terminal type"
pc_test
tb b "1 10 +10 +10" || pc_fail "tabs '1 10 +10 +10' failed"
cmp -s a b || pc_fail "1,10,20,30 and '1 10 +10 +10' differ"
tb c 1,10,+10,+10 || pc_fail "tabs 1,10,+10,+10 failed"
cmp -s a c || pc_fail "1,10,20,30 and 1,10,+10,+10 differ"
tb d 1,10,+10,30 || pc_fail "a list mixing increments and absolute values failed"
cmp -s a d || pc_fail "an increment and the equal absolute value differ"
pc_pass
