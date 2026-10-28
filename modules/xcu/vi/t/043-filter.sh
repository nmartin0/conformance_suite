# ! filters lines through a shell command
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.vi.043
#
. "$PC_CORE/pc.sh"
. "$PC_CORE/pcpty.sh"
pc_begin XCU.vi.043
pc_pty_need
pc_pty_term
HOME=$PWD; export HOME
unset EXINIT
LC_ALL=POSIX; export LC_ALL
printf 'abc\ndef\nghi\njkl\nmno\n' >f
printf '!!tr a-z A-Z\n2!!tr a-z A-Z\n:4\n!Gsort -r\n:wq\n' >in
pc_test
pc_vi_run in f
[ "$pc_pty_rc" -eq 0 ] || pc_fail "vi exited with status $pc_pty_rc"
printf 'ABC\nDEF\nghi\nmno\njkl\n' >want
cmp -s f want || pc_fail "the file is: $(tr '\n' '|' <f)"
pc_pass
