# wait with no operands waits for every child and exits 0.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.wait.001
. "$PC_CORE/pc.sh"
pc_begin XCU.wait.001
pc_test
( sleep 1; : >wdone ) &
"${PC_SH:-sh}" -c 'exit 3' &
wait
rc=$?
[ "$rc" -eq 0 ] || pc_fail "wait exited $rc, wanted 0"
[ -f wdone ] || pc_fail "wait returned before the slow child had finished"
pc_pass
