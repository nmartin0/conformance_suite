# :w with a file name writes that file only, and :q! quits without writing
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.vi.052
#
. "$PC_CORE/pc.sh"
. "$PC_CORE/pcpty.sh"
pc_begin XCU.vi.052
pc_pty_need
pc_pty_term
HOME=$PWD; export HOME
unset EXINIT
printf 'ff\n' >f
printf 'x\n:w out\n\033:q!\n' >in
pc_test
pc_vi_run in f
[ "$pc_pty_rc" -eq 0 ] || pc_fail "vi exited with status $pc_pty_rc"
printf 'ff\n' >want
cmp -s f want || pc_fail "the file is: $(tr '\n' '|' <f)"
printf 'f\n' >want.out
cmp -s out want.out || pc_fail "file out is: $(tr '\n' '|' <out)"
pc_pass
