# yy and p, x and p
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.vi.005
#
. "$PC_CORE/pc.sh"
. "$PC_CORE/pcpty.sh"
pc_begin XCU.vi.005
pc_pty_need
pc_pty_term
printf 'ab\ncd\n' >f
{ printf 'yypxp\n'; printf ':wq\n'; } >in
pc_test
pc_vi_run in f
[ "$pc_pty_rc" -eq 0 ] || pc_fail "vi exited with status $pc_pty_rc"
printf 'ab\nba\ncd\n' >want
# yyp puts a copy of line 1 below it with the cursor on the copy; x deletes
# its first character and p puts that character back after the next one.
cmp -s f want || pc_fail "the file is: $(tr '\n' '|' <f)"
pc_pass
