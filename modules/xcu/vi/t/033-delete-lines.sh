# dd, dj, dk, d+, d-, dG and d_ delete whole lines
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.vi.033
#
. "$PC_CORE/pc.sh"
. "$PC_CORE/pcpty.sh"
pc_begin XCU.vi.033
pc_pty_need
pc_pty_term
HOME=$PWD; export HOME
unset EXINIT
printf 'a1\na2\na3\na4\na5\na6\na7\na8\na9\na10\na11\na12\na13\na14\na15\na16\n' >f
printf '2dd:3\ndj:3\ndk:4\nd+:3\nd-:2\nd4G:2\nd_:wq\n' >in
pc_test
pc_vi_run in f
[ "$pc_pty_rc" -eq 0 ] || pc_fail "vi exited with status $pc_pty_rc"
printf 'a3\na16\n' >want
cmp -s f want || pc_fail "the file is: $(tr '\n' '|' <f)"
pc_pass
