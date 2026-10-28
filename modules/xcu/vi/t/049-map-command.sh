# :map defines a command-mode mapping, applied again to its result
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.vi.049
#
. "$PC_CORE/pc.sh"
. "$PC_CORE/pcpty.sh"
pc_begin XCU.vi.049
pc_pty_need
pc_pty_term
HOME=$PWD; export HOME
unset EXINIT
printf 'one\ntwo\nthree\n' >f
printf ':map K dd\n\033K\n:map v K\n\033v\n:wq\n' >in
pc_test
pc_vi_run in f
[ "$pc_pty_rc" -eq 0 ] || pc_fail "vi exited with status $pc_pty_rc"
printf 'two\n' >want
cmp -s f want || pc_fail "the file is: $(tr '\n' '|' <f)"
pc_pass
