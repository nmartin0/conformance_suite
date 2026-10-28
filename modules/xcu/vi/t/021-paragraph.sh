# } and { move to paragraph boundaries
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.vi.021
#
. "$PC_CORE/pc.sh"
. "$PC_CORE/pcpty.sh"
pc_begin XCU.vi.021
pc_pty_need
pc_pty_term
HOME=$PWD; export HOME
unset EXINIT
printf 'aa\nbb\n\ncc\ndd\n\nee\nff\n\ngg\n' >f
printf '}jx:1\n2}jx:10\n{kx:10\n2{kx:wq\n' >in
pc_test
pc_vi_run in f
[ "$pc_pty_rc" -eq 0 ] || pc_fail "vi exited with status $pc_pty_rc"
printf 'aa\nbb\n\nc\nd\n\ne\nf\n\ngg\n' >want
cmp -s f want || pc_fail "the file is: $(tr '\n' '|' <f)"
pc_pass
