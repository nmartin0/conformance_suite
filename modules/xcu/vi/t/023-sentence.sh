# ) and ( move to sentence beginnings
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.vi.023
#
. "$PC_CORE/pc.sh"
. "$PC_CORE/pcpty.sh"
pc_begin XCU.vi.023
pc_pty_need
pc_pty_term
HOME=$PWD; export HOME
unset EXINIT
printf 'Aa.  Bb!  Cc?  Dd.\nAa.  Bb!  Cc?  Dd.\nAa.  Bb!  Cc?  Dd.\nAa.  Bb!  Cc?  Dd.\n' >f
printf ')x\n^2)x\n$(x\n$2(x\n:wq\n' >in
pc_test
pc_vi_run in f
[ "$pc_pty_rc" -eq 0 ] || pc_fail "vi exited with status $pc_pty_rc"
printf 'Aa.  b!  Cc?  Dd.\nAa.  Bb!  c?  Dd.\nAa.  Bb!  Cc?  d.\nAa.  Bb!  c?  Dd.\n' >want
cmp -s f want || pc_fail "the file is: $(tr '\n' '|' <f)"
pc_pass
