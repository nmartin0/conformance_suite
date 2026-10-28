# ZZ writes and exits
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.vi.007
#
. "$PC_CORE/pc.sh"
. "$PC_CORE/pcpty.sh"
pc_begin XCU.vi.007
pc_pty_need
pc_pty_term
printf 'one\ntwo\n' >f
{ printf 'dd\n'; printf 'ZZ\n'; } >in
pc_test
pc_vi_run in f
[ "$pc_pty_rc" -eq 0 ] || pc_fail "vi exited with status $pc_pty_rc"
printf 'two\n' >want
cmp -s f want || pc_fail "ZZ did not write the edited file: $(tr '\n' '|' <f)"
pc_pass
