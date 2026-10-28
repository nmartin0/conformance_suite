# ]] and [[ move to section boundaries
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.vi.022
#
. "$PC_CORE/pc.sh"
. "$PC_CORE/pcpty.sh"
pc_begin XCU.vi.022
pc_pty_need
pc_pty_term
HOME=$PWD; export HOME
unset EXINIT
printf 'a1\na2\n{\nb1\nb2\n.SH x\nc1\nc2\n{\nd1\nd2\n' >f
printf ']]jx:1\n2]]jx:1\n3]]jx:11\n[[kx:11\n2[[kx:11\n3[[kx:wq\n' >in
pc_test
pc_vi_run in f
[ "$pc_pty_rc" -eq 0 ] || pc_fail "vi exited with status $pc_pty_rc"
printf 'a1\n2\n{\n1\n2\n.SH x\n1\n2\n{\n1\nd2\n' >want
cmp -s f want || pc_fail "the file is: $(tr '\n' '|' <f)"
pc_pass
