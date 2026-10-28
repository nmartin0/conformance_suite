# bg fails when job control is disabled.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.bg.001
. "$PC_CORE/pc.sh"
pc_begin XCU.bg.001
p=
trap 'kill -s KILL "$p" 2>/dev/null' EXIT
set +m || pc_fail "set +m is not accepted"
pc_test
sleep 101 &
p=$!
bg >out 2>/dev/null
rc=$?
[ "$rc" -gt 0 ] || pc_fail "bg exited $rc with job control disabled"
[ ! -s out ] || pc_fail "bg wrote to standard output with job control disabled"
pc_pass
