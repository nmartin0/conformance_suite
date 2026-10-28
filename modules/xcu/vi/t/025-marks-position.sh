# m sets a mark that the apostrophe and grave accent commands return to
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.vi.025
#
. "$PC_CORE/pc.sh"
. "$PC_CORE/pcpty.sh"
pc_begin XCU.vi.025
pc_pty_need
pc_pty_term
HOME=$PWD; export HOME
unset EXINIT
printf 'abcdef\nabcdef\nabcdef\nabcdef\nabcdef\n' >f
printf ':2\n^llmq:3\n^lllmr:5\n'\''qx:5\n`rx:wq\n' >in
pc_test
pc_vi_run in f
[ "$pc_pty_rc" -eq 0 ] || pc_fail "vi exited with status $pc_pty_rc"
printf 'abcdef\nbcdef\nabcef\nabcdef\nabcdef\n' >want
cmp -s f want || pc_fail "the file is: $(tr '\n' '|' <f)"
pc_pass
