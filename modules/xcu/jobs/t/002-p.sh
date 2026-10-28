# jobs -p writes the process group leader's ID.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.jobs.002
. "$PC_CORE/pc.sh"
pc_begin XCU.jobs.002
p=
trap 'kill -s KILL "$p" 2>/dev/null' EXIT
nl='
'
set -m || pc_fail "set -m is not accepted"
pc_test
sleep 101 &
p=$!
jobs -p >out 2>/dev/null
rc=$?
[ "$rc" -eq 0 ] || pc_fail "jobs -p exited $rc"
got=$(cat out; echo x)
[ "$got" = "$p${nl}x" ] || pc_fail "jobs -p wrote something other than $p and a newline"
pc_pass
