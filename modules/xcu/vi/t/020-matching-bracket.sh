# % moves to the matching parenthesis, bracket or brace
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.vi.020
#
. "$PC_CORE/pc.sh"
. "$PC_CORE/pcpty.sh"
pc_begin XCU.vi.020
pc_pty_need
pc_pty_term
HOME=$PWD; export HOME
unset EXINIT
printf 'a(b[c]d)e\nx{a(b)c}y\nab(cd)ef\n(p(q)r)s\n{\nx\n}\n' >f
printf '^f(%%x\n^f{%%x\n^%%x\n$F)%%x\n^%%x\n:wq\n' >in
pc_test
pc_vi_run in f
[ "$pc_pty_rc" -eq 0 ] || pc_fail "vi exited with status $pc_pty_rc"
printf 'a(b[c]de\nx{a(b)cy\nab(cdef\np(q)r)s\n{\nx\n\n' >want
cmp -s f want || pc_fail "the file is: $(tr '\n' '|' <f)"
pc_pass
