# several operands are processed in order
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.tput.004
#
. "$PC_CORE/pc.sh"
. "$PC_CORE/pcpty.sh"
pc_begin XCU.tput.004
pc_pty_need
pc_pty_term
for op in clear init; do
	"$PC_PTY" -t 20 -o "one.$op" -- env "TERM=$PCT" tput "$op" || pc_unresolved "tput $op failed on $PCT"
done
pc_test
"$PC_PTY" -t 20 -o both -- env "TERM=$PCT" tput clear init
rc=$?
[ "$rc" -eq 0 ] || pc_fail "tput clear init exited with status $rc"
cat one.clear one.init >joined
cmp -s both joined || pc_fail "the output for two operands is not the two outputs in order"
pc_pass
