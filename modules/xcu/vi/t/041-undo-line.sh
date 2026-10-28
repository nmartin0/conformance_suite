# u undoes (and undoes itself); U restores the current line
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.vi.041
#
. "$PC_CORE/pc.sh"
. "$PC_CORE/pcpty.sh"
pc_begin XCU.vi.041
pc_pty_need
pc_pty_term
HOME=$PWD; export HOME
unset EXINIT
printf 'abcdef\nabcdef\nabcdef\nabc\n' >f
printf 'xxxU\nAxy\033U\nx\nxuu\n:wq\n' >in
pc_test
pc_vi_run in f
[ "$pc_pty_rc" -eq 0 ] || pc_fail "vi exited with status $pc_pty_rc"
printf 'abcdef\nabcdef\nbcdef\nbc\n' >want
cmp -s f want || pc_fail "the file is: $(tr '\n' '|' <f)"
pc_pass
