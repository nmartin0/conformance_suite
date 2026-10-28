# c with motions, C, s, S, cc, r and R, ~
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.vi.034
#
. "$PC_CORE/pc.sh"
. "$PC_CORE/pcpty.sh"
pc_begin XCU.vi.034
pc_pty_need
pc_pty_term
HOME=$PWD; export HOME
unset EXINIT
printf 'one two three\none two three\none two three\none two three\none two three\none two three\none two three\none two three\none two three\none two three\na,b,c\na,b,c\nabcdef\nabcdef\nabc\naBc dEf\n' >f
printf 'cwX\033\n^2cwX\033\n^wCX\033\n^wc$X\033\n^sX\033\n^3sX\033\nSX\033\nccX\033\n^wcbX\033\n$c0X\033\n^cf,X\033\n^ct,X\033\n^3rX\n^Rxy\033\n^lrX\n^7~\n:wq\n' >in
pc_test
pc_vi_run in f
[ "$pc_pty_rc" -eq 0 ] || pc_fail "vi exited with status $pc_pty_rc"
printf 'X two three\nX three\none X\none X\nXne two three\nX two three\nX\nX\nXtwo three\nXe\nXb,c\nX,b,c\nXXXdef\nxycdef\naXc\nAbC DeF\n' >want
cmp -s f want || pc_fail "the file is: $(tr '\n' '|' <f)"
pc_pass
