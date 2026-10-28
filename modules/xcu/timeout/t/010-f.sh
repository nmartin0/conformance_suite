# -f is accepted.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.timeout.010
. "$PC_CORE/pc.sh"
pc_begin XCU.timeout.010
pc_test
timeout -f 1 sleep 30 </dev/null >/dev/null 2>&1
rc=$?
[ "$rc" -eq 124 ] || pc_fail "timeout -f 1 sleep 30 exited $rc, wanted 124"
timeout -f 30 "${PC_SH:-sh}" -c 'echo ok; exit 2' </dev/null >out 2>/dev/null
rc=$?
[ "$rc" -eq 2 ] || pc_fail "timeout -f 30 exited $rc, wanted the utility's status 2"
got=$(cat out)
[ "$got" = ok ] || pc_fail "timeout -f 30 wrote '$got', wanted ok"
pc_pass
