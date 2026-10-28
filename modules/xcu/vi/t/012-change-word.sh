# cw, count and w
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.vi.012
#
. "$PC_CORE/pc.sh"
. "$PC_CORE/pcpty.sh"
pc_begin XCU.vi.012
pc_pty_need
pc_pty_term
printf 'alpha beta gamma\n' >f
{ printf 'wcwXY\033\n'; printf '2x\n'; printf ':wq\n'; } >in
pc_test
pc_vi_run in f
[ "$pc_pty_rc" -eq 0 ] || pc_fail "vi exited with status $pc_pty_rc"
# w moves to beta; cw replaces it with XY, leaving the cursor on Y; 2x
# deletes two characters from there: "Y" and the blank.
printf 'alpha Xgamma\n' >want
cmp -s f want || pc_fail "the file is: $(tr '\n' '|' <f)"
pc_pass
