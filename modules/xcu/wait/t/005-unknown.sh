# A process ID already waited for is unknown: exit 127.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.wait.005
. "$PC_CORE/pc.sh"
pc_begin XCU.wait.005
pc_test
"${PC_SH:-sh}" -c 'exit 0' &
p=$!
wait "$p"
rc=$?
[ "$rc" -eq 0 ] || pc_fail "first wait exited $rc, wanted 0"
wait "$p" 2>/dev/null
rc=$?
[ "$rc" -eq 127 ] || pc_fail "second wait exited $rc, wanted 127"
pc_pass
