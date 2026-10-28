# a usage error exits with status 2
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.tput.006
#
. "$PC_CORE/pc.sh"
. "$PC_CORE/pcpty.sh"
pc_begin XCU.tput.006
pc_pty_need
pc_pty_term
pc_test
"$PC_PTY" -t 20 -o out -- env "TERM=$PCT" tput
rc=$?
[ "$rc" -ne 125 ] || pc_unresolved "cannot set up a pseudo-terminal"
[ "$rc" -eq 2 ] || pc_fail "tput without operands exited with status $rc, not 2"
"$PC_PTY" -t 20 -o out2 -- env "TERM=$PCT" tput -Q clear
rc=$?
[ "$rc" -eq 2 ] || pc_fail "tput with an unknown option exited with status $rc, not 2"
pc_pass
