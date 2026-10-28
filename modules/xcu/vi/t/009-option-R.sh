# -R opens the file read-only
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.vi.009
#
. "$PC_CORE/pc.sh"
. "$PC_CORE/pcpty.sh"
pc_begin XCU.vi.009
pc_pty_need
pc_pty_term
printf 'a\nb\n' >f
{ printf 'dd\n'; printf ':w\n'; printf ':q!\n'; } >in
pc_test
pc_vi_run in -R f
[ "$pc_pty_rc" -eq 0 ] || pc_fail "vi -R exited with status $pc_pty_rc"
printf 'a\nb\n' >want
cmp -s f want || pc_fail "vi -R let the file be overwritten: $(tr '\n' '|' <f)"
pc_pass
