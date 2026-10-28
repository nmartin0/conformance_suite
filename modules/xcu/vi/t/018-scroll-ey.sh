# control-E and control-Y scroll the display by lines
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.vi.018
#
. "$PC_CORE/pc.sh"
. "$PC_CORE/pcpty.sh"
pc_begin XCU.vi.018
pc_pty_need
pc_pty_term
HOME=$PWD; export HOME
unset EXINIT
printf '  L01\n  L02\n  L03\n  L04\n  L05\n  L06\n  L07\n  L08\n  L09\n  L10\n  L11\n  L12\n  L13\n  L14\n  L15\n  L16\n  L17\n  L18\n  L19\n  L20\n' >f
printf '^\005rM:2\n^\005rN:3\n^3\005rO:6\n^\031rP:6\n^4\031rQ:wq\n' >in
pc_test
"$PC_PTY" -t 20 -d 400 -i in -o pty.out -- env "TERM=$PCT" LINES=6 COLUMNS=80 \
    "${PC_SH:-sh}" -c 'stty rows 6 cols 80; exec vi -w 5 "$@"' vi f
pc_pty_rc=$?
[ "$pc_pty_rc" -ne 125 ] || pc_unresolved "cannot set up a pseudo-terminal"
[ "$pc_pty_rc" -ne 124 ] || pc_fail "vi did not finish"
[ "$pc_pty_rc" -eq 0 ] || pc_fail "vi exited with status $pc_pty_rc"
printf '  Q01\n  M02\n  N03\n  L04\n  L05\n  P06\n  L07\n  L08\n  L09\n  L10\n  L11\n  L12\n  L13\n  L14\n  L15\n  L16\n  L17\n  L18\n  L19\n  L20\n' >want
cmp -s f want || pc_fail "the file is: $(tr '\n' '|' <f)"
pc_pass
