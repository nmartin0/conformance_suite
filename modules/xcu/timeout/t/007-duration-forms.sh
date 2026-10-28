# Fractions and suffixes in the duration.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.timeout.007
. "$PC_CORE/pc.sh"
pc_begin XCU.timeout.007
pc_test
timeout 0.5s sleep 30 </dev/null >/dev/null 2>&1
rc=$?
[ "$rc" -eq 124 ] || pc_fail "timeout 0.5s sleep 30 exited $rc, wanted 124"
timeout 0.5 sleep 30 </dev/null >/dev/null 2>&1
rc=$?
[ "$rc" -eq 124 ] || pc_fail "timeout 0.5 sleep 30 exited $rc, wanted 124"
for d in 1s 1m 1h 1d 30 2.5 0.25m; do
	timeout "$d" "${PC_SH:-sh}" -c 'exit 5' </dev/null >/dev/null 2>&1
	rc=$?
	[ "$rc" -eq 5 ] || pc_fail "timeout $d on a short utility exited $rc, wanted its status 5"
done
pc_pass
