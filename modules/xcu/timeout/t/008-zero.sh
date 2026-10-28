# A duration of zero means no limit.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.timeout.008
. "$PC_CORE/pc.sh"
pc_begin XCU.timeout.008
pc_test
for d in 0 0s 0.0; do
	timeout "$d" "${PC_SH:-sh}" -c 'sleep 2; echo done; exit 4' </dev/null >out 2>/dev/null
	rc=$?
	[ "$rc" -eq 4 ] || pc_fail "timeout $d exited $rc, wanted the utility's status 4"
	got=$(cat out)
	[ "$got" = done ] || pc_fail "timeout $d stopped the utility (it wrote '$got')"
done
pc_pass
