# wait does not return before the process ends.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.wait.003
. "$PC_CORE/pc.sh"
pc_begin XCU.wait.003
pc_test
( sleep 1; : >wdone; exit 5 ) &
p=$!
wait "$p"
rc=$?
[ -f wdone ] || pc_fail "wait returned before the process had finished"
[ "$rc" -eq 5 ] || pc_fail "wait exited $rc, wanted 5"
pc_pass
