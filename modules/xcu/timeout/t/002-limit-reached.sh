# At the limit SIGTERM is sent and the status is 124.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.timeout.002
. "$PC_CORE/pc.sh"
pc_begin XCU.timeout.002
pc_test
# 3 seconds leave the child time to install its trap; the loop is bounded
# so that a timeout that never signals cannot hang the test
timeout 3 "${PC_SH:-sh}" -c 'trap "echo term; exit 9" TERM; n=0; while [ $n -lt 20 ]; do sleep 1; n=$((n+1)); done' </dev/null >out 2>/dev/null
rc=$?
[ "$rc" -eq 124 ] || pc_fail "timeout exited $rc, wanted 124"
got=$(cat out)
[ "$got" = term ] || pc_fail "the utility did not get SIGTERM (it wrote '$got')"
timeout 1 sleep 30 </dev/null >/dev/null 2>&1
rc=$?
[ "$rc" -eq 124 ] || pc_fail "timeout 1 sleep 30 exited $rc, wanted 124"
pc_pass
