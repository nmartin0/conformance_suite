# a mark as a motion: d with the apostrophe (lines) and the grave accent (characters)
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.vi.026
#
. "$PC_CORE/pc.sh"
. "$PC_CORE/pcpty.sh"
pc_begin XCU.vi.026
pc_pty_need
pc_pty_term
HOME=$PWD; export HOME
unset EXINIT
printf 'one\ntwo\nthree\nfour\nfive\nabcdef\n' >f
printf ':2\nmq:4\nd'\''q:3\n^lllmq$d`q:wq\n' >in
pc_test
pc_vi_run in f
[ "$pc_pty_rc" -eq 0 ] || pc_fail "vi exited with status $pc_pty_rc"
printf 'one\nfive\nabcf\n' >want
cmp -s f want || pc_fail "the file is: $(tr '\n' '|' <f)"
pc_pass
