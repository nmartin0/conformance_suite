# :e file edits another file
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.vi.053
#
. "$PC_CORE/pc.sh"
. "$PC_CORE/pcpty.sh"
pc_begin XCU.vi.053
pc_pty_need
pc_pty_term
HOME=$PWD; export HOME
unset EXINIT
printf 'ff\n' >f
printf '  gg\n  hh\n  ii\n' >g
printf 'x\n:w\n:e g\n\033x\n:wq\n' >in
pc_test
pc_vi_run in f
[ "$pc_pty_rc" -eq 0 ] || pc_fail "vi exited with status $pc_pty_rc"
printf 'f\n' >want
cmp -s f want || pc_fail "the file is: $(tr '\n' '|' <f)"
printf '  g\n  hh\n  ii\n' >want.g
cmp -s g want.g || pc_fail "file g is: $(tr '\n' '|' <g)"
pc_pass
