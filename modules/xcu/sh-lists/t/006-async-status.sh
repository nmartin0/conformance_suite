# The status of an asynchronous list as seen when the shell goes on is zero;
# its real status is available through wait on $!.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.sh-lists.006
. "$PC_CORE/pc.sh"
pc_begin XCU.sh-lists.006
pc_test
(exit 7) &
rc=$?
pid=$!
[ "$rc" -eq 0 ] || pc_fail "status of the asynchronous list was $rc"
[ -n "$pid" ] || pc_fail "\$! is empty"
wait "$pid"
w=$?
[ "$w" -eq 7 ] || pc_fail "wait returned $w, expected 7"
pc_pass
