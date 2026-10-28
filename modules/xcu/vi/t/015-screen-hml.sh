# H, M and L move to lines of the display
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.vi.015
#
. "$PC_CORE/pc.sh"
. "$PC_CORE/pcpty.sh"
pc_begin XCU.vi.015
pc_pty_need
pc_pty_term
HOME=$PWD; export HOME
unset EXINIT
printf '  L01\n  L02\n  L03\n  L04\n  L05\n  L06\n  L07\n  L08\n  L09\n  L10\n  L11\n  L12\n  L13\n  L14\n  L15\n  L16\n  L17\n  L18\n  L19\n  L20\n' >f
printf 'Lx:3\nHx:3\nMx:3\n2Hx:3\n2Lx:wq\n' >in
pc_test
"$PC_PTY" -t 20 -d 400 -i in -o pty.out -- env "TERM=$PCT" LINES=6 COLUMNS=80 \
    "${PC_SH:-sh}" -c 'stty rows 6 cols 80; exec vi -w 5 "$@"' vi f
pc_pty_rc=$?
[ "$pc_pty_rc" -ne 125 ] || pc_unresolved "cannot set up a pseudo-terminal"
[ "$pc_pty_rc" -ne 124 ] || pc_fail "vi did not finish"
[ "$pc_pty_rc" -eq 0 ] || pc_fail "vi exited with status $pc_pty_rc"
printf '  01\n  02\n  03\n  04\n  05\n  L06\n  L07\n  L08\n  L09\n  L10\n  L11\n  L12\n  L13\n  L14\n  L15\n  L16\n  L17\n  L18\n  L19\n  L20\n' >want
cmp -s f want || pc_fail "the file is: $(tr '\n' '|' <f)"
pc_pass
