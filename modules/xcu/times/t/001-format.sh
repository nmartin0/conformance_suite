# times writes two lines in the form NmN.NNs NmN.NNs, the first for the
# shell's user and system times and the second for its children, and exits
# with status zero.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.times.001
. "$PC_CORE/pc.sh"
pc_begin XCU.times.001
pc_test
times >pc.t 2>pc.e
rc=$?
[ "$rc" -eq 0 ] || pc_fail "times exited with status $rc"
[ -s pc.e ] && pc_fail "times wrote to standard error"
n=0
while IFS= read -r l; do
	n=$((n + 1))
done <pc.t
[ "$n" -eq 2 ] || pc_fail "times wrote $n lines, expected 2"
c=$(grep -c -E '^[0-9]+m[0-9]+(\.[0-9]*)?s [0-9]+m[0-9]+(\.[0-9]*)?s$' pc.t)
[ "$c" -eq 2 ] || pc_fail "lines do not have the form NmN.NNs NmN.NNs: $(cat pc.t)"
pc_pass
