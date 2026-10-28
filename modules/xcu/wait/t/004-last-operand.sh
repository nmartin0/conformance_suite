# With several operands wait gives the status of the last.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.wait.004
. "$PC_CORE/pc.sh"
pc_begin XCU.wait.004
pc_test
( sleep 1; : >w1; exit 3 ) &
p1=$!
"${PC_SH:-sh}" -c 'exit 4' &
p2=$!
wait "$p1" "$p2"
rc=$?
[ -f w1 ] || pc_fail "wait returned before the first process had finished"
[ "$rc" -eq 4 ] || pc_fail "wait exited $rc, wanted 4 (status of the last operand)"
pc_pass
