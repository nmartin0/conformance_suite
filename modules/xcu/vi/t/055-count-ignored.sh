# a count given to D, ^, % or u is ignored
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.vi.055
#
. "$PC_CORE/pc.sh"
. "$PC_CORE/pcpty.sh"
pc_begin XCU.vi.055
pc_pty_need
pc_pty_term
HOME=$PWD; export HOME
unset EXINIT
printf 'one two\n  abc\na(b)c\nabcdef\n' >f
printf 'w3D\n$2^x\n^f(5%%x\nx2u\n:wq\n' >in
pc_test
pc_vi_run in f
[ "$pc_pty_rc" -eq 0 ] || pc_fail "vi exited with status $pc_pty_rc"
printf 'one \n  bc\na(bc\nabcdef\n' >want
cmp -s f want || pc_fail "the file is: $(tr '\n' '|' <f)"
pc_pass
