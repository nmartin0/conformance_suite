# escape ends a line-oriented command and runs it; control-H on an empty line cancels it
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.vi.060
#
. "$PC_CORE/pc.sh"
. "$PC_CORE/pcpty.sh"
pc_begin XCU.vi.060
pc_pty_need
pc_pty_term
HOME=$PWD; export HOME
unset EXINIT
printf 'abc\n' >f
printf ':s/b/Y/\033\n:\010x\n:wq\n' >in
pc_test
pc_vi_run in f
[ "$pc_pty_rc" -eq 0 ] || pc_fail "vi exited with status $pc_pty_rc"
printf 'Yc\n' >want
cmp -s f want || pc_fail "the file is: $(tr '\n' '|' <f)"
pc_pass
