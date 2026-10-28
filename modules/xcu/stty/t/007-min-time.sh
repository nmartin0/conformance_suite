# min and time operands round-trip
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.stty.007
#
. "$PC_CORE/pc.sh"
. "$PC_CORE/pcpty.sh"
pc_begin XCU.stty.007
pc_pty_need
cat >inner.sh <<'IN'
stty -icanon min 3 time 5 || exit 1
stty -g >g1.out; stty -a >a1.out
stty min 4 time 6 || exit 2
stty -g >g2.out
stty min 3 time 5 || exit 3
stty -g >g3.out
exit 0
IN
pc_test
pc_pty_run inner.sh
[ "$pc_pty_rc" -eq 0 ] || pc_fail "step $pc_pty_rc failed"
cmp -s g1.out g2.out && pc_fail "min and time had no effect"
cmp -s g1.out g3.out || pc_fail "min 3 time 5 did not restore the earlier state"
pc_pass
