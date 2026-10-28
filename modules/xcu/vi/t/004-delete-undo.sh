# x, dd and u
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.vi.004
#
. "$PC_CORE/pc.sh"
. "$PC_CORE/pcpty.sh"
pc_begin XCU.vi.004
pc_pty_need
pc_pty_term
printf 'abc\nline2\nline3\n' >f
{ printf 'x\n'; printf 'dd\n'; printf 'u\n'; printf ':wq\n'; } >in
pc_test
pc_vi_run in f
[ "$pc_pty_rc" -eq 0 ] || pc_fail "vi exited with status $pc_pty_rc"
printf 'bc\nline2\nline3\n' >want
cmp -s f want || pc_fail "after x, dd and u the file is: $(tr '\n' '|' <f)"
pc_pass
