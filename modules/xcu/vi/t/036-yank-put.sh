# y, Y, p and P with line and character regions and counts
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.vi.036
#
. "$PC_CORE/pc.sh"
. "$PC_CORE/pcpty.sh"
pc_begin XCU.vi.036
pc_pty_need
pc_pty_term
HOME=$PWD; export HOME
unset EXINIT
printf 'aa\nbb\ncc\ndd\nee\nxy zw\nxyz\n' >f
printf 'yyp:4\n2yyP:3\nY2p:11\n^yw$p0P:12\n^yl3p:wq\n' >in
pc_test
pc_vi_run in f
[ "$pc_pty_rc" -eq 0 ] || pc_fail "vi exited with status $pc_pty_rc"
printf 'aa\naa\nbb\nbb\nbb\ncc\ndd\ncc\ndd\nee\nxy xy zwxy \nxxxxyz\n' >want
cmp -s f want || pc_fail "the file is: $(tr '\n' '|' <f)"
pc_pass
