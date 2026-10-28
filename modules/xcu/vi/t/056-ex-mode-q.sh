# Q leaves visual mode for ex mode, where ex commands run line by line
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.vi.056
#
. "$PC_CORE/pc.sh"
. "$PC_CORE/pcpty.sh"
pc_begin XCU.vi.056
pc_pty_need
pc_pty_term
HOME=$PWD; export HOME
unset EXINIT
printf 'aaa\n' >f
printf 'Q\ns/a/b/\nwq\n' >in
pc_test
pc_vi_run in f
[ "$pc_pty_rc" -eq 0 ] || pc_fail "vi exited with status $pc_pty_rc"
printf 'baa\n' >want
cmp -s f want || pc_fail "the file is: $(tr '\n' '|' <f)"
pc_pass
