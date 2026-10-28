# J, r and . repeat
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.vi.010
#
. "$PC_CORE/pc.sh"
. "$PC_CORE/pcpty.sh"
pc_begin XCU.vi.010
pc_pty_need
pc_pty_term
printf 'abcdef\none\ntwo\n' >f
{ printf 'x.rZjJ\n'; printf ':wq\n'; } >in
pc_test
pc_vi_run in f
[ "$pc_pty_rc" -eq 0 ] || pc_fail "vi exited with status $pc_pty_rc"
# x. deletes two characters, rZ replaces the next one, j goes down and J
# joins the last two lines with a blank.
printf 'Zdef
one two
' >want
cmp -s f want || pc_fail "the file is: $(tr '\n' '|' <f)"
pc_pass
