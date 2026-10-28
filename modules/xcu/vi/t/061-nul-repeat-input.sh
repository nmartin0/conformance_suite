# NUL as first character of input mode inserts the previous input text and leaves input mode
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.vi.061
#
. "$PC_CORE/pc.sh"
. "$PC_CORE/pcpty.sh"
pc_begin XCU.vi.061
pc_pty_need
pc_pty_term
HOME=$PWD; export HOME
unset EXINIT
printf 'abc\n' >f
printf 'ihello\033\nA\000\n:wq\n' >in
pc_test
pc_vi_run in f
[ "$pc_pty_rc" -eq 0 ] || pc_fail "vi exited with status $pc_pty_rc"
printf 'helloabchello\n' >want
cmp -s f want || pc_fail "the file is: $(tr '\n' '|' <f)"
pc_pass
