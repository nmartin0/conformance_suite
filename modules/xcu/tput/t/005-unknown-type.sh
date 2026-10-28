# an unknown terminal type exits with status 3
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.tput.005
#
. "$PC_CORE/pc.sh"
. "$PC_CORE/pcpty.sh"
pc_begin XCU.tput.005
pc_pty_need
pc_test
"$PC_PTY" -t 20 -o out -- env "TERM=pc-no-such-terminal-type" tput clear
rc=$?
[ "$rc" -ne 125 ] || pc_unresolved "cannot set up a pseudo-terminal"
[ "$rc" -eq 3 ] || pc_fail "tput on an unknown terminal type exited with status $rc, not 3"
"$PC_PTY" -t 20 -o out2 -- tput -T pc-no-such-terminal-type clear
rc=$?
[ "$rc" -eq 3 ] || pc_fail "tput -T with an unknown type exited with status $rc, not 3"
pc_pass
