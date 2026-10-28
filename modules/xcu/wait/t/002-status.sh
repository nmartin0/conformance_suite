# wait pid exits with the status of that process.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.wait.002
. "$PC_CORE/pc.sh"
pc_begin XCU.wait.002
pc_test
"${PC_SH:-sh}" -c 'exit 3' &
p=$!
wait "$p"
rc=$?
[ "$rc" -eq 3 ] || pc_fail "wait exited $rc, wanted 3"
pc_pass
