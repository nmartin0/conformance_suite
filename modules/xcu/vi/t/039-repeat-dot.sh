# . repeats the last change, replacing the count
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.vi.039
#
. "$PC_CORE/pc.sh"
. "$PC_CORE/pcpty.sh"
pc_begin XCU.vi.039
pc_pty_need
pc_pty_term
HOME=$PWD; export HOME
unset EXINIT
printf 'a b c d e f\na b c d e f\nabcdef\nabc abc\nabc\nabc\nabc\nabcd\np\nq\nr\ns\n' >f
printf 'dw.\n^dw3.\n^x2.\n^cwX\033w.\n^ix\033.\nAx\033.\n^~.\n^rXl.\nJ.\n:wq\n' >in
pc_test
pc_vi_run in f
[ "$pc_pty_rc" -eq 0 ] || pc_fail "vi exited with status $pc_pty_rc"
printf 'c d e f\ne f\ndef\nX X\nxxabc\nabcxx\nABc\nXXcd\np q r\ns\n' >want
cmp -s f want || pc_fail "the file is: $(tr '\n' '|' <f)"
pc_pass
