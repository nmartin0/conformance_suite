# control-^ edits the alternate file
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.vi.054
#
. "$PC_CORE/pc.sh"
. "$PC_CORE/pcpty.sh"
pc_begin XCU.vi.054
pc_pty_need
pc_pty_term
HOME=$PWD; export HOME
unset EXINIT
printf 'ff\n' >f
printf 'gg\n' >g
printf ':e g\n\033rG\n:w\n\036\n\033rF\n:wq\n' >in
pc_test
pc_vi_run in f
[ "$pc_pty_rc" -eq 0 ] || pc_fail "vi exited with status $pc_pty_rc"
printf 'Ff\n' >want
cmp -s f want || pc_fail "the file is: $(tr '\n' '|' <f)"
printf 'Gg\n' >want.g
cmp -s g want.g || pc_fail "file g is: $(tr '\n' '|' <g)"
pc_pass
