# dd then :q! leaves the file unchanged
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.vi.006
#
. "$PC_CORE/pc.sh"
. "$PC_CORE/pcpty.sh"
pc_begin XCU.vi.006
pc_pty_need
pc_pty_term
printf 'keep1\nkeep2\n' >f
{ printf 'dd\n'; printf ':q!\n'; } >in
pc_test
pc_vi_run in f
[ "$pc_pty_rc" -eq 0 ] || pc_fail "vi exited with status $pc_pty_rc"
printf 'keep1\nkeep2\n' >want
cmp -s f want || pc_fail "the file was changed by a discarded edit"
pc_pass
