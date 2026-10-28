# An asynchronous list and a command substitution run in a subshell, so
# their variable changes do not reach the shell.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.sh-environment.008
. "$PC_CORE/pc.sh"
pc_begin XCU.sh-environment.008
PCV=orig
PCW=orig
pc_test
{ PCV=changed; } &
wait
[ "$PCV" = orig ] || pc_fail "asynchronous list changed PCV to '$PCV'"
r=$(PCW=changed; echo in)
[ "$r" = in ] || pc_fail "substitution output '$r'"
[ "$PCW" = orig ] || pc_fail "command substitution changed PCW to '$PCW'"
pc_pass
