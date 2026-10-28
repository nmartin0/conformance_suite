# the colon command line runs an ex command
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.vi.011
#
. "$PC_CORE/pc.sh"
. "$PC_CORE/pcpty.sh"
pc_begin XCU.vi.011
pc_pty_need
pc_pty_term
printf 'foo bar\nfoo baz\n' >f
{ printf ':%%s/foo/qux/\n'; printf ':wq\n'; } >in
pc_test
pc_vi_run in f
[ "$pc_pty_rc" -eq 0 ] || pc_fail "vi exited with status $pc_pty_rc"
printf 'qux bar\nqux baz\n' >want
cmp -s f want || pc_fail "the file is: $(tr '\n' '|' <f)"
pc_pass
