# a search used as a motion deletes whole lines or characters as the match position decides
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.vi.030
#
. "$PC_CORE/pc.sh"
. "$PC_CORE/pcpty.sh"
pc_begin XCU.vi.030
pc_pty_need
pc_pty_term
HOME=$PWD; export HOME
unset EXINIT
printf 'ab cd ef\none\ntwo\nthree\nfour\nfive\nend\n' >f
printf 'd/cd\n:2\nd/two/+1\n:1\n^d/four\n:2\n^d?four\n:wq\n' >in
pc_test
pc_vi_run in f
[ "$pc_pty_rc" -eq 0 ] || pc_fail "vi exited with status $pc_pty_rc"
printf 'five\nend\n' >want
cmp -s f want || pc_fail "the file is: $(tr '\n' '|' <f)"
pc_pass
