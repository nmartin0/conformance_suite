# -p keeps the utility's own status.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.timeout.004
. "$PC_CORE/pc.sh"
pc_begin XCU.timeout.004
pc_test
timeout -p 3 "${PC_SH:-sh}" -c 'trap "exit 9" TERM; n=0; while [ $n -lt 20 ]; do sleep 1; n=$((n+1)); done; exit 8' </dev/null >/dev/null 2>&1
rc=$?
[ "$rc" -eq 9 ] || pc_fail "timeout -p exited $rc, wanted the utility's status 9"
timeout 3 "${PC_SH:-sh}" -c 'trap "exit 9" TERM; n=0; while [ $n -lt 20 ]; do sleep 1; n=$((n+1)); done; exit 8' </dev/null >/dev/null 2>&1
rc=$?
[ "$rc" -eq 124 ] || pc_fail "timeout without -p exited $rc, wanted 124"
pc_pass
