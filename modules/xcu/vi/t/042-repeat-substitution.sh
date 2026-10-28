# & repeats the last substitution on the current line without its flags
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.vi.042
#
. "$PC_CORE/pc.sh"
. "$PC_CORE/pcpty.sh"
pc_begin XCU.vi.042
pc_pty_need
pc_pty_term
HOME=$PWD; export HOME
unset EXINIT
printf 'aa\naa\naa\naa\n' >f
printf ':s/a/X/g\nj&\n&\n:wq\n' >in
pc_test
pc_vi_run in f
[ "$pc_pty_rc" -eq 0 ] || pc_fail "vi exited with status $pc_pty_rc"
printf 'XX\nXa\nXa\naa\n' >want
cmp -s f want || pc_fail "the file is: $(tr '\n' '|' <f)"
pc_pass
