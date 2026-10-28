# autoindent, control-T, control-D and the removal of unused indent on escape
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.vi.047
#
. "$PC_CORE/pc.sh"
. "$PC_CORE/pcpty.sh"
pc_begin XCU.vi.047
pc_pty_need
pc_pty_term
HOME=$PWD; export HOME
EXINIT='set autoindent shiftwidth=2'; export EXINIT
printf '    a\n    b\n    c\n    d\n' >f
printf ':4\no\004X\033:3\no\024X\033:2\no0\004X\033:1\nofoo\n\033:wq\n' >in
pc_test
pc_vi_run in f
[ "$pc_pty_rc" -eq 0 ] || pc_fail "vi exited with status $pc_pty_rc"
printf '    a\n    foo\n\n    b\nX\n    c\n      X\n    d\n  X\n' >want
cmp -s f want || pc_fail "the file is: $(tr '\n' '|' <f)"
pc_pass
