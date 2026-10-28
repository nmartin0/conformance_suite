# w, W, b, B, e and E move by words and bigwords
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.vi.019
#
. "$PC_CORE/pc.sh"
. "$PC_CORE/pcpty.sh"
pc_begin XCU.vi.019
pc_pty_need
pc_pty_term
HOME=$PWD; export HOME
unset EXINIT
printf 'ab-cd ef\nab-cd ef\nab-cd ef\nab-cd ef\nab-cd ef\nab-cd ef\nab-cd ef\nab-cd ef\nab-cd ef\n' >f
printf 'wx\n^Wx\n^lex\n^4lEx\n^2wx\n$bx\n$2Bx\n$99Bx:9\n^9Wx\n^9Ex\n:wq\n' >in
pc_test
pc_vi_run in f
[ "$pc_pty_rc" -eq 0 ] || pc_fail "vi exited with status $pc_pty_rc"
printf 'bcd ef\nab-cd f\nabcd ef\nab-cd e\nab-d ef\nab-cd f\nb-cd ef\nab-cd ef\nab-cd \n' >want
cmp -s f want || pc_fail "the file is: $(tr '\n' '|' <f)"
pc_pass
