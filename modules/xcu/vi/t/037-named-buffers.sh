# named buffers take yanks and deletes, an upper-case name appends, and the unnamed buffer also receives the text
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.vi.037
#
. "$PC_CORE/pc.sh"
. "$PC_CORE/pcpty.sh"
pc_begin XCU.vi.037
pc_pty_need
pc_pty_term
HOME=$PWD; export HOME
unset EXINIT
printf 'one\ntwo\nthree\nfour\n' >f
printf '"ayy:2\n"Ayy:3\n^"by$:4\n"ap"bp:6\n"cddp"cp:wq\n' >in
pc_test
pc_vi_run in f
[ "$pc_pty_rc" -eq 0 ] || pc_fail "vi exited with status $pc_pty_rc"
printf 'one\ntwo\nthree\nfour\nothreene\ntwo\ntwo\n' >want
cmp -s f want || pc_fail "the file is: $(tr '\n' '|' <f)"
pc_pass
