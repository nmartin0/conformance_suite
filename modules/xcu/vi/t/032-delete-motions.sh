# d with word, line-end, character and bracket motions
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.vi.032
#
. "$PC_CORE/pc.sh"
. "$PC_CORE/pcpty.sh"
pc_begin XCU.vi.032
pc_pty_need
pc_pty_term
HOME=$PWD; export HOME
unset EXINIT
printf 'a b c d e f\na b c d e f\na b c d e f\na b c d e f\none two three\none two three\none two three\none two three\na,b,c\na,b,c\na,b,c\na,b,c\na,bcd\nx(y)z\none two\nabcdef\nabc\nabc def\nab cd\n' >f
printf 'dw\n^2dw\n^d2w\n^2d2w\n^wD\n^wd$\n$d0\n^wd^\n^df,\n^dt,\n^d2f,\n$dF,\n$dT,\n^f(d%%\n^4dl\n$2dh\n$dl\n^wdb\n$dB\n:wq\n' >in
pc_test
pc_vi_run in f
[ "$pc_pty_rc" -eq 0 ] || pc_fail "vi exited with status $pc_pty_rc"
printf 'b c d e f\nc d e f\nc d e f\ne f\none \none \ne\ntwo three\nb,c\n,b,c\nc\na,bc\na,d\nxz\ntwo\nabcf\nab\ndef\nab d\n' >want
cmp -s f want || pc_fail "the file is: $(tr '\n' '|' <f)"
pc_pass
