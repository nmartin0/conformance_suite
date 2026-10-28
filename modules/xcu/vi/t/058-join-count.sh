# J joins count lines (one more than count minus one), following the ex join rules
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.vi.058
#
. "$PC_CORE/pc.sh"
. "$PC_CORE/pcpty.sh"
pc_begin XCU.vi.058
pc_pty_need
pc_pty_term
HOME=$PWD; export HOME
unset EXINIT
printf 'a\nb\nc\nd\ne\nf\ng.\nh\n' >f
printf '3J:2\n2J:4\nJ:wq\n' >in
pc_test
pc_vi_run in f
[ "$pc_pty_rc" -eq 0 ] || pc_fail "vi exited with status $pc_pty_rc"
printf 'a b c\nd e\nf\ng.  h\n' >want
cmp -s f want || pc_fail "the file is: $(tr '\n' '|' <f)"
pc_pass
