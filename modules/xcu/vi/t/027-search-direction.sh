# /, ?, n and N search with the remembered direction
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.vi.027
#
. "$PC_CORE/pc.sh"
. "$PC_CORE/pcpty.sh"
pc_begin XCU.vi.027
pc_pty_need
pc_pty_term
HOME=$PWD; export HOME
EXINIT='set wrapscan'; export EXINIT
printf 'foo 1\nbar\nfoo 2\nbaz\nfoo 3\nqux\nend\n' >f
printf '/bar\nAa\033:1\n/foo\nnAb\033:7\n?baz\nAc\033:7\n?foo\nnAd\033:7\n?foo\nNAe\033:4\n/foo\nNAf\033:wq\n' >in
pc_test
pc_vi_run in f
[ "$pc_pty_rc" -eq 0 ] || pc_fail "vi exited with status $pc_pty_rc"
printf 'foo 1e\nbara\nfoo 2df\nbazc\nfoo 3b\nqux\nend\n' >want
cmp -s f want || pc_fail "the file is: $(tr '\n' '|' <f)"
pc_pass
