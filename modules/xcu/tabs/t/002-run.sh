# tabs with a valid terminal type succeeds or diagnoses
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.tabs.002
#
# The page lets tabs fail for a terminal that cannot be set up, but then it
# must give a diagnostic and a nonzero status; it never fails silently.
. "$PC_CORE/pc.sh"
. "$PC_CORE/pcpty.sh"
pc_begin XCU.tabs.002
pc_pty_need
pc_pty_term
export PCT
pc_test
for a in "-T $PCT" "-T $PCT -8" "-T $PCT 1,9,17"; do
	"$PC_PTY" -t 20 -o out -- env "TERM=$PCT" tabs $a
	rc=$?
	[ "$rc" -ne 125 ] || pc_unresolved "cannot set up a pseudo-terminal"
	[ "$rc" -ne 124 ] || pc_unresolved "tabs did not finish"
	if [ "$rc" -ne 0 ] && [ ! -s out ]; then pc_fail "tabs $a failed (status $rc) without a diagnostic"; fi
done
pc_pass
