# a newline typed in the middle of a line splits it
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.vi.066
#
. "$PC_CORE/pc.sh"
. "$PC_CORE/pcpty.sh"
pc_begin XCU.vi.066
pc_pty_need
pc_pty_term
HOME=$PWD; export HOME
unset EXINIT
printf 'abcd\n' >f
printf 'llixy\nz\033\n:wq\n' >in
pc_test
pc_vi_run in f
[ "$pc_pty_rc" -eq 0 ] || pc_fail "vi exited with status $pc_pty_rc"
printf 'abxy\nzcd\n' >want
cmp -s f want || pc_fail "the file is: $(tr '\n' '|' <f)"
pc_pass
