# control-H, control-W, control-U and control-V in text input mode
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.vi.046
#
. "$PC_CORE/pc.sh"
. "$PC_CORE/pcpty.sh"
pc_begin XCU.vi.046
pc_pty_need
pc_pty_term
HOME=$PWD; export HOME
unset EXINIT
printf 'abc\n' >f
printf 'ofoo bar baz\027X\033\nofoo bar  \027X\033\nofoo bar\025X\033\nofoo\010\010X\033\nox\026\010y\033:1\nAfoo bar\027X\033\n:wq\n' >in
pc_test
pc_vi_run in f
[ "$pc_pty_rc" -eq 0 ] || pc_fail "vi exited with status $pc_pty_rc"
printf 'abcfoo X\nfoo bar X\nfoo X\nX\nfX\nx\010y\n' >want
cmp -s f want || pc_fail "the file is: $(tr '\n' '|' <f)"
pc_pass
