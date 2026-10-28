# tabs rejects an unknown option
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.tabs.006
#
. "$PC_CORE/pc.sh"
. "$PC_CORE/pcpty.sh"
pc_begin XCU.tabs.006
pc_pty_need
pc_pty_term
pc_test
"$PC_PTY" -t 20 -o out -- env "TERM=$PCT" tabs -Q
rc=$?
[ "$rc" -ne 125 ] || pc_unresolved "cannot set up a pseudo-terminal"
[ "$rc" -gt 0 ] && [ "$rc" -ne 124 ] || pc_fail "tabs -Q exited with status $rc"
pc_pass
