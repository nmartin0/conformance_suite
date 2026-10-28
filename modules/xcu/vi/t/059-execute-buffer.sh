# @ runs the contents of a buffer as commands, and @@ repeats it
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.vi.059
#
. "$PC_CORE/pc.sh"
. "$PC_CORE/pcpty.sh"
pc_begin XCU.vi.059
pc_pty_need
pc_pty_term
HOME=$PWD; export HOME
unset EXINIT
printf 'one two three four five six\ndw\nabc\n' >f
printf ':2\n"ayy:1\n^@ax:1\n^@@:1\n^2@a:wq\n' >in
pc_test
pc_vi_run in f
[ "$pc_pty_rc" -eq 0 ] || pc_fail "vi exited with status $pc_pty_rc"
printf 'five six\nw\nabc\n' >want
cmp -s f want || pc_fail "the file is: $(tr '\n' '|' <f)"
pc_pass
