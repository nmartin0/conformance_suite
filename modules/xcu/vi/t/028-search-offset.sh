# a search can end with a line offset, and an empty pattern reuses the last one
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.vi.028
#
. "$PC_CORE/pc.sh"
. "$PC_CORE/pcpty.sh"
pc_begin XCU.vi.028
pc_pty_need
pc_pty_term
HOME=$PWD; export HOME
unset EXINIT
printf 'foo 1\nbar\nfoo 2\nbaz\nfoo 3\nqux\nend\n' >f
printf '/qux/-1\nAf\033:1\n/bar/+2\nAg\033:1\n/baz/0\nAh\033:1\n/foo\n/\nAi\033:wq\n' >in
pc_test
pc_vi_run in f
[ "$pc_pty_rc" -eq 0 ] || pc_fail "vi exited with status $pc_pty_rc"
printf 'foo 1\nbar\nfoo 2\nbazgh\nfoo 3fi\nqux\nend\n' >want
cmp -s f want || pc_fail "the file is: $(tr '\n' '|' <f)"
pc_pass
