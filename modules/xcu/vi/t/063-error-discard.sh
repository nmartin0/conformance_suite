# an escape or an error discards a partly entered command
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.vi.063
#
. "$PC_CORE/pc.sh"
. "$PC_CORE/pcpty.sh"
pc_begin XCU.vi.063
pc_pty_need
pc_pty_term
HOME=$PWD; export HOME
unset EXINIT
printf 'abcd\nabcd\nabcd\n' >f
printf 'd\033x\n3\033x\ndfzx\n:wq\n' >in
pc_test
pc_vi_run in f
[ "$pc_pty_rc" -eq 0 ] || pc_fail "vi exited with status $pc_pty_rc"
printf 'bcd\nbcd\nbcd\n' >want
cmp -s f want || pc_fail "the file is: $(tr '\n' '|' <f)"
pc_pass
