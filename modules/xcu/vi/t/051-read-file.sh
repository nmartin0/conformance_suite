# :r reads a file or a command output below the current line
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.vi.051
#
. "$PC_CORE/pc.sh"
. "$PC_CORE/pcpty.sh"
pc_begin XCU.vi.051
pc_pty_need
pc_pty_term
HOME=$PWD; export HOME
unset EXINIT
printf 'a\nb\n' >f
printf 'r1\nr2\n' >g
printf ':r g\n\033:r !echo sh1\n:wq\n' >in
pc_test
pc_vi_run in f
[ "$pc_pty_rc" -eq 0 ] || pc_fail "vi exited with status $pc_pty_rc"
printf 'a\nr1\nsh1\nr2\nb\n' >want
cmp -s f want || pc_fail "the file is: $(tr '\n' '|' <f)"
pc_pass
