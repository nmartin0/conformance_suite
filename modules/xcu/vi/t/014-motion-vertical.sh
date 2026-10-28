# j, k, +, -, _ and G move between lines
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.vi.014
#
. "$PC_CORE/pc.sh"
. "$PC_CORE/pcpty.sh"
pc_begin XCU.vi.014
pc_pty_need
pc_pty_term
HOME=$PWD; export HOME
unset EXINIT
printf 'A1\nB2\n  C3\nD4\nE5\nF6\nG7\n  H8\nI9\nJ10\nK11\n  L12\nM13\n  N14\n' >f
printf '^l3jx:7\n^lkx:1\n3_x:6\n2+x:13\n-x:11\n2-x:5\n_x:13\n13Gx:1\nGx:wq\n' >in
pc_test
pc_vi_run in f
[ "$pc_pty_rc" -eq 0 ] || pc_fail "vi exited with status $pc_pty_rc"
printf 'A1\nB2\n  3\nD\n5\nF\nG7\n  8\n9\nJ10\nK11\n  12\n13\n  14\n' >want
cmp -s f want || pc_fail "the file is: $(tr '\n' '|' <f)"
pc_pass
