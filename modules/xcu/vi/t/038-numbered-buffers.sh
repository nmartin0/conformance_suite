# deleted lines go to numbered buffers 1 to 9, shifting; small deletes do not
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.vi.038
#
. "$PC_CORE/pc.sh"
. "$PC_CORE/pcpty.sh"
pc_begin XCU.vi.038
pc_pty_need
pc_pty_term
HOME=$PWD; export HOME
unset EXINIT
printf 'a1\na2\na3\na4\na5\na6\n' >f
# the newline that ends a typed line moves the cursor down, so k brings it
# back to the line the puts are meant to follow
printf 'dddddd\nk"1p"2p"3p:5\n^x"1p:wq\n' >in
pc_test
pc_vi_run in f
[ "$pc_pty_rc" -eq 0 ] || pc_fail "vi exited with status $pc_pty_rc"
printf 'a4\na3\na2\na1\n5\na3\na6\n' >want
cmp -s f want || pc_fail "the file is: $(tr '\n' '|' <f)"
pc_pass
