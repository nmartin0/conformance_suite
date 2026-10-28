# . after a put from a numbered buffer uses the next higher buffer
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.vi.040
#
. "$PC_CORE/pc.sh"
. "$PC_CORE/pcpty.sh"
pc_begin XCU.vi.040
pc_pty_need
pc_pty_term
HOME=$PWD; export HOME
unset EXINIT
printf 'a1\na2\na3\na4\na5\na6\n' >f
printf 'dddddd\n"1pu.u.\n:wq\n' >in
pc_test
pc_vi_run in f
[ "$pc_pty_rc" -eq 0 ] || pc_fail "vi exited with status $pc_pty_rc"
printf 'a4\na1\na5\na6\n' >want
cmp -s f want || pc_fail "the file is: $(tr '\n' '|' <f)"
pc_pass
