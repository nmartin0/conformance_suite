# insert, append and open commands
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.vi.003
#
. "$PC_CORE/pc.sh"
. "$PC_CORE/pcpty.sh"
pc_begin XCU.vi.003
pc_pty_need
pc_pty_term
printf 'abc\nxyz\n' >f
{ printf 'ihello\033Aend\033onew\033\n'; printf ':wq\n'; } >in
pc_test
pc_vi_run in f
[ "$pc_pty_rc" -eq 0 ] || pc_fail "vi exited with status $pc_pty_rc"
printf 'helloabcend\nnew\nxyz\n' >want
cmp -s f want || pc_fail "the edited file is not as expected: $(tr '\n' '|' <f)"
pc_pass
