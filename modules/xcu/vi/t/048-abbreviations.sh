# an abbreviation expands after a word in text input and not inside a longer word
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.vi.048
#
. "$PC_CORE/pc.sh"
. "$PC_CORE/pcpty.sh"
pc_begin XCU.vi.048
pc_pty_need
pc_pty_term
HOME=$PWD; export HOME
EXINIT='ab gm good morning'; export EXINIT
printf 'start\n' >f
printf 'ogm today\033\nogm\033\noxgm y\033\n:unab gm\n\033ogm z\033\n:ab hh hello world\n\033ohh\033\n:wq\n' >in
pc_test
pc_vi_run in f
[ "$pc_pty_rc" -eq 0 ] || pc_fail "vi exited with status $pc_pty_rc"
printf 'start\ngood morning today\ngood morning\nxgm y\ngm z\nhello world\n' >want
cmp -s f want || pc_fail "the file is: $(tr '\n' '|' <f)"
pc_pass
