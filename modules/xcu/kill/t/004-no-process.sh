# kill of a process that does not exist fails.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.kill.004
. "$PC_CORE/pc.sh"
pc_begin XCU.kill.004
"${PC_SH:-sh}" -c 'exit 0' &
d=$!
wait "$d"
pc_test
kill "$d" >out 2>/dev/null
rc=$?
[ "$rc" -gt 0 ] || pc_fail "kill exited $rc for a process that is gone"
[ ! -s out ] || pc_fail "kill wrote to standard output"
pc_pass
