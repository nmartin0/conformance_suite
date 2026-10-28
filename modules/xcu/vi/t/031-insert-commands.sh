# a, A, i, I, o and O with counts
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.vi.031
#
. "$PC_CORE/pc.sh"
. "$PC_CORE/pcpty.sh"
pc_begin XCU.vi.031
pc_pty_need
pc_pty_term
HOME=$PWD; export HOME
unset EXINIT
printf 'abc\nabc\nabc\nabc\n  abc\nabc\nabc\nabc\nabc\nabc\nabc\n' >f
printf 'lazz\033\n^3az\033\n^lizz\033\n^2ix\033\nIzz\033\n2Ix\033\nAzz\033\n2Az\033\nox\033\n2oy\033\nOw\033\n2Oz\033\n:wq\n' >in
pc_test
pc_vi_run in f
[ "$pc_pty_rc" -eq 0 ] || pc_fail "vi exited with status $pc_pty_rc"
printf 'abzzc\nazzzbc\nazzbc\nxxabc\n  zzabc\nxxabc\nabczz\nabczz\nabc\nx\nabc\ny\ny\nw\nz\nz\nabc\n' >want
cmp -s f want || pc_fail "the file is: $(tr '\n' '|' <f)"
pc_pass
