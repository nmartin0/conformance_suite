# control-D and control-U scroll by the scroll amount or the last count
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.vi.017
#
. "$PC_CORE/pc.sh"
. "$PC_CORE/pcpty.sh"
pc_begin XCU.vi.017
pc_pty_need
pc_pty_term
HOME=$PWD; export HOME
EXINIT='set scroll=3'; export EXINIT
printf '  L01\n  L02\n  L03\n  L04\n  L05\n  L06\n  L07\n  L08\n  L09\n  L10\n  L11\n  L12\n  L13\n  L14\n  L15\n  L16\n  L17\n  L18\n  L19\n  L20\n' >f
printf '\004rD:4\n\004rE:7\n2\004rF:9\n\004rG:11\n\025rH:9\n5\025rI:4\n\025rJ:1\n99\004rK:wq\n' >in
pc_test
pc_vi_run in f
[ "$pc_pty_rc" -eq 0 ] || pc_fail "vi exited with status $pc_pty_rc"
printf '  J01\n  L02\n  L03\n  I04\n  L05\n  L06\n  E07\n  L08\n  H09\n  L10\n  G11\n  L12\n  L13\n  L14\n  L15\n  L16\n  L17\n  L18\n  L19\n  K20\n' >want
cmp -s f want || pc_fail "the file is: $(tr '\n' '|' <f)"
pc_pass
