# < and > shift lines by the shiftwidth
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.vi.044
#
. "$PC_CORE/pc.sh"
. "$PC_CORE/pcpty.sh"
pc_begin XCU.vi.044
pc_pty_need
pc_pty_term
HOME=$PWD; export HOME
EXINIT='set shiftwidth=2'; export EXINIT
printf 'a\nb\nc\nd\ne\n  f\n    g\ni\n' >f
printf '>>:2\n2>>:4\n>j:6\n<<:7\n<<:8\n>>.:wq\n' >in
pc_test
pc_vi_run in f
[ "$pc_pty_rc" -eq 0 ] || pc_fail "vi exited with status $pc_pty_rc"
printf '  a\n  b\n  c\n  d\n  e\nf\n  g\n    i\n' >want
cmp -s f want || pc_fail "the file is: $(tr '\n' '|' <f)"
pc_pass
