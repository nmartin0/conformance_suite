# XSI preset tab lists equal their stop lists
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.tabs.007
#
. "$PC_CORE/pc.sh"
. "$PC_CORE/pcpty.sh"
pc_begin XCU.tabs.007
pc_pty_need
pc_pty_term
tb() { o=$1; shift; TERM=$PCT "$PC_PTY" -t 20 -o "$o" -- tabs "$@"; }
tb ref 1,10,20 || pc_unresolved "setting explicit tab stops is not possible on this terminal type"
pc_test
for p in "-a:1,10,16,36,72" "-a2:1,10,16,40,72" "-c:1,8,12,16,20,55" "-c2:1,6,10,14,49" \
	"-c3:1,6,10,14,18,22,26,30,34,38,42,46,50,54,58,62,67" "-f:1,7,11,15,19,23" \
	"-p:1,5,9,13,17,21,25,29,33,37,41,45,49,53,57,61" "-s:1,10,55" "-u:1,12,20,44"; do
	opt=${p%%:*}; list=${p#*:}
	tb x "$opt" || pc_fail "tabs $opt failed"
	[ -s x ] || pc_unresolved "tabs wrote nothing to the terminal, so the presets cannot be compared"
	tb y "$list" || pc_fail "tabs $list failed"
	cmp -s x y || pc_fail "tabs $opt differs from tabs $list"
done
pc_pass
