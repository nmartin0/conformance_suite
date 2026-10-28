# A utility that ends in time is passed through.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.timeout.001
. "$PC_CORE/pc.sh"
pc_begin XCU.timeout.001
pc_test
timeout 30 "${PC_SH:-sh}" -c 'echo "$1 $2"; exit 3' sh a 'b c' </dev/null >out 2>/dev/null
rc=$?
[ "$rc" -eq 3 ] || pc_fail "timeout exited $rc, wanted the utility's status 3"
got=$(cat out)
[ "$got" = "a b c" ] || pc_fail "the utility wrote '$got', wanted 'a b c'"
timeout 30 true </dev/null >/dev/null 2>&1
rc=$?
[ "$rc" -eq 0 ] || pc_fail "timeout 30 true exited $rc"
(exec timeout 30 "${PC_SH:-sh}" -c 'exit 2') </dev/null >/dev/null 2>&1
rc=$?
[ "$rc" -eq 2 ] || pc_fail "exec timeout sh -c 'exit 2' exited $rc, wanted 2"
pc_pass
