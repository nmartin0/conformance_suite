# -s names the signal to send.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.timeout.003
. "$PC_CORE/pc.sh"
pc_begin XCU.timeout.003
pc_test
for name in USR1 usr1; do
	timeout -s "$name" 3 "${PC_SH:-sh}" -c 'trap "echo usr1; exit 0" USR1; n=0; while [ $n -lt 20 ]; do sleep 1; n=$((n+1)); done' </dev/null >out 2>/dev/null
	rc=$?
	[ "$rc" -eq 124 ] || pc_fail "timeout -s $name exited $rc, wanted 124"
	got=$(cat out)
	[ "$got" = usr1 ] || pc_fail "timeout -s $name did not send USR1 (the utility wrote '$got')"
done
timeout -s KILL 1 sleep 30 </dev/null >/dev/null 2>&1
rc=$?
[ "$rc" -eq 124 ] || pc_fail "timeout -s KILL exited $rc, wanted 124"
pc_pass
