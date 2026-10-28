# a line number before z with dot, hyphen or newline makes that line current
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.vi.057
#
. "$PC_CORE/pc.sh"
. "$PC_CORE/pcpty.sh"
pc_begin XCU.vi.057
pc_pty_need
pc_pty_term
HOME=$PWD; export HOME
unset EXINIT
printf 'q01\nq02\nq03\nq04\nq05\nq06\nq07\nq08\nq09\nq10\nq11\nq12\n' >f
printf '4z.x:1\n7z-x:1\n6z\nx:9\nz.x:1\n99z.x:wq\n' >in
pc_test
pc_vi_run in f
[ "$pc_pty_rc" -eq 0 ] || pc_fail "vi exited with status $pc_pty_rc"
printf 'q01\nq02\nq03\n04\nq05\n06\n07\nq08\n09\nq10\nq11\n12\n' >want
cmp -s f want || pc_fail "the file is: $(tr '\n' '|' <f)"
pc_pass
