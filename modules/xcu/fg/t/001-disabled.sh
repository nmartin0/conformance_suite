# fg fails when job control is disabled.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.fg.001
. "$PC_CORE/pc.sh"
pc_begin XCU.fg.001
p=
trap 'kill -s KILL "$p" 2>/dev/null' EXIT
set +m || pc_fail "set +m is not accepted"
pc_test
sleep 101 &
p=$!
fg >/dev/null 2>&1
rc=$?
[ "$rc" -gt 0 ] || pc_fail "fg exited $rc with job control disabled"
pc_pass
