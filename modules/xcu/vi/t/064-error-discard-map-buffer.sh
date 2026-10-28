# an error stops the rest of a map expansion and of an executed buffer
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.vi.064
#
. "$PC_CORE/pc.sh"
. "$PC_CORE/pcpty.sh"
pc_begin XCU.vi.064
pc_pty_need
pc_pty_term
HOME=$PWD; export HOME
unset EXINIT
printf 'abc\ndef\nfzdd\n' >f
printf ':map K fzdd\n\033K\n:3\n"ayy:1\n@a:1\n^x\n:wq\n' >in
pc_test
pc_vi_run in f
[ "$pc_pty_rc" -eq 0 ] || pc_fail "vi exited with status $pc_pty_rc"
printf 'bc\ndef\nfzdd\n' >want
cmp -s f want || pc_fail "the file is: $(tr '\n' '|' <f)"
pc_pass
