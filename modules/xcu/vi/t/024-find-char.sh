# f, F, t, T, ; and , find a character on the line
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.vi.024
#
. "$PC_CORE/pc.sh"
. "$PC_CORE/pcpty.sh"
pc_begin XCU.vi.024
pc_pty_need
pc_pty_term
HOME=$PWD; export HOME
unset EXINIT
printf 'a,b,c,d,e\na,b,c,d,e\na,b,c,d,e\na,b,c,d,e\na,b,c,d,e\na,b,c,d,e\na,b,c,d,e\n' >f
printf 'f,x\n2f,x\n$F,x\n2t,x\n$2T,x\nf,;x\nf,;,x\n:wq\n' >in
pc_test
pc_vi_run in f
[ "$pc_pty_rc" -eq 0 ] || pc_fail "vi exited with status $pc_pty_rc"
printf 'ab,c,d,e\na,bc,d,e\na,b,c,de\na,,c,d,e\na,b,c,,e\na,bc,d,e\nab,c,d,e\n' >want
cmp -s f want || pc_fail "the file is: $(tr '\n' '|' <f)"
pc_pass
