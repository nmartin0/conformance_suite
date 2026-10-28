# vi :wq keeps the file and exits zero
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.vi.002
#
. "$PC_CORE/pc.sh"
. "$PC_CORE/pcpty.sh"
pc_begin XCU.vi.002
pc_pty_need
pc_pty_term
printf 'one\ntwo\nthree\n' >f
printf ':wq\n' >in
pc_test
pc_vi_run in f
[ "$pc_pty_rc" -eq 0 ] || pc_fail "vi exited with status $pc_pty_rc"
printf 'one\ntwo\nthree\n' >want
cmp -s f want || pc_fail "the file changed although nothing was edited"
pc_pass
