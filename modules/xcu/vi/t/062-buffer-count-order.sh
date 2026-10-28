# a buffer name and a count may come in either order
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.vi.062
#
. "$PC_CORE/pc.sh"
. "$PC_CORE/pcpty.sh"
pc_begin XCU.vi.062
pc_pty_need
pc_pty_term
HOME=$PWD; export HOME
unset EXINIT
printf 'x\n' >f
printf '"ayy\n2"ap\n"a2p\n:wq\n' >in
pc_test
pc_vi_run in f
[ "$pc_pty_rc" -eq 0 ] || pc_fail "vi exited with status $pc_pty_rc"
printf 'x\nx\nx\nx\nx\n' >want
cmp -s f want || pc_fail "the file is: $(tr '\n' '|' <f)"
pc_pass
