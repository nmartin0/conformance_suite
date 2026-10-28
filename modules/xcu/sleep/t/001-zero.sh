# sleep 0 returns at once with status zero.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.sleep.001
. "$PC_CORE/pc.sh"
pc_begin XCU.sleep.001
pc_test
sleep 0 </dev/null >out 2>/dev/null
rc=$?
[ "$rc" -eq 0 ] || pc_fail "sleep 0 exited $rc"
[ ! -s out ] || pc_fail "sleep 0 wrote to standard output: $(cat out)"
(exec sleep 0) </dev/null >out 2>/dev/null
rc=$?
[ "$rc" -eq 0 ] || pc_fail "exec sleep 0 exited $rc"
[ ! -s out ] || pc_fail "exec sleep 0 wrote to standard output"
pc_pass
