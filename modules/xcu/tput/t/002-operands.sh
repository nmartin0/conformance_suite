# tput clear, init and reset succeed on a known terminal type
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.tput.002
#
. "$PC_CORE/pc.sh"
. "$PC_CORE/pcpty.sh"
pc_begin XCU.tput.002
pc_pty_need
pc_pty_term
pc_test
for op in clear init reset; do
	"$PC_PTY" -t 20 -o "out.$op" -- env "TERM=$PCT" tput "$op"
	rc=$?
	[ "$rc" -ne 125 ] || pc_unresolved "cannot set up a pseudo-terminal"
	[ "$rc" -eq 0 ] || pc_fail "tput $op exited with status $rc"
	# the clear-screen sequence of a known type is not empty (a utility
	# that does nothing is not told apart by its exit status alone)
	[ "$op" != clear ] || [ -s "out.$op" ] || pc_fail "tput clear on $PCT wrote no clear-screen sequence"
done
pc_pass
