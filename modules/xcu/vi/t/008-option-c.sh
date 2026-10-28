# -c runs an ex command after the first read
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.vi.008
#
. "$PC_CORE/pc.sh"
. "$PC_CORE/pcpty.sh"
pc_begin XCU.vi.008
pc_pty_need
pc_pty_term
printf 'a\nb\nc\n' >f
printf ':wq\n' >in
pc_test
pc_vi_run in -c 1d f
[ "$pc_pty_rc" -eq 0 ] || pc_fail "vi exited with status $pc_pty_rc"
printf 'b\nc\n' >want
cmp -s f want || pc_fail "the file after -c 1d is: $(tr '\n' '|' <f)"
pc_pass
