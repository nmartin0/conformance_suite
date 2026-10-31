# -R opens the file read-only
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.vi.009
#
. "$PC_CORE/pc.sh"
. "$PC_CORE/pcpty.sh"
pc_begin XCU.vi.009
pc_pty_need
pc_pty_term
printf 'a\nb\n' >f
{ printf 'dd\n'; printf ':w\n'; printf ':q!\n'; } >in
pc_test
pc_vi_run in -R f
# The script has a command that fails (:w on a file opened read-only), so
# the exit status of vi is not tested: it may report that an error occurred.
printf 'a\nb\n' >want
cmp -s f want || pc_fail "vi -R let the file be overwritten: $(tr '\n' '|' <f)"
pc_pass
