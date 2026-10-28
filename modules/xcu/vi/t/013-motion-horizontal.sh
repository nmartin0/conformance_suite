# h, l, space, control-H, 0, $, ^ and | move the cursor
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.vi.013
#
. "$PC_CORE/pc.sh"
. "$PC_CORE/pcpty.sh"
pc_begin XCU.vi.013
pc_pty_need
pc_pty_term
HOME=$PWD; export HOME
unset EXINIT
printf 'abcdefgh\nabcdefgh\nabcdefgh\n  abcdef\nabcdefgh\nabcdefgh\nabcdefgh\nabcdefgh\nabc\nabc\n' >f
printf '3lx\n$hx\n$0x\n$^x\n5|x\n$3hx\n^2 x\n$\010\010x\n^99lx\n$99hx\n:wq\n' >in
pc_test
pc_vi_run in f
[ "$pc_pty_rc" -eq 0 ] || pc_fail "vi exited with status $pc_pty_rc"
printf 'abcefgh\nabcdefh\nbcdefgh\n  bcdef\nabcdfgh\nabcdfgh\nabdefgh\nabcdegh\nab\nbc\n' >want
cmp -s f want || pc_fail "the file is: $(tr '\n' '|' <f)"
pc_pass
