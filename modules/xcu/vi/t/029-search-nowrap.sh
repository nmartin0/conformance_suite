# with wrapscan unset a search does not wrap and a failed search leaves the cursor
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.vi.029
#
. "$PC_CORE/pc.sh"
. "$PC_CORE/pcpty.sh"
pc_begin XCU.vi.029
pc_pty_need
pc_pty_term
HOME=$PWD; export HOME
EXINIT='set nowrapscan'; export EXINIT
printf 'foo1\nbar\nfoo3\n' >f
printf ':3\n/foo\nAa\033:1\n?foo\nAb\033:wq\n' >in
pc_test
pc_vi_run in f
[ "$pc_pty_rc" -eq 0 ] || pc_fail "vi exited with status $pc_pty_rc"
printf 'foo1b\nbar\nfoo3a\n' >want
cmp -s f want || pc_fail "the file is: $(tr '\n' '|' <f)"
pc_pass
