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
# a kill that fails for every process would also pass: it must work on a live one
kill -s 0 $$ >/dev/null 2>&1 || pc_unresolved "kill -s 0 fails for the running shell, so its failure for a missing process proves nothing"
kill "$d" >out 2>/dev/null
rc=$?
[ "$rc" -gt 0 ] || pc_fail "kill exited $rc for a process that is gone"
[ ! -s out ] || pc_fail "kill wrote to standard output"
# the same through a direct exec of the utility
env kill -s 0 $$ >/dev/null 2>&1 || pc_unresolved "env kill -s 0 fails for the running shell, so its failure for a missing process proves nothing"
env kill "$d" >out 2>/dev/null
rc=$?
[ "$rc" -gt 0 ] || pc_fail "env kill exited $rc for a process that is gone"
[ ! -s out ] || pc_fail "env kill wrote to standard output"
pc_pass
