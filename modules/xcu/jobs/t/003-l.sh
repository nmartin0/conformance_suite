# jobs -l inserts the process group ID before the state.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.jobs.003
. "$PC_CORE/pc.sh"
pc_begin XCU.jobs.003
p=
trap 'kill -s KILL "$p" 2>/dev/null' EXIT
set -m || pc_fail "set -m is not accepted"
pc_test
sleep 101 &
p=$!
jobs -l >out 2>/dev/null
rc=$?
[ "$rc" -eq 0 ] || pc_fail "jobs -l exited $rc"
read -r f1 f2 f3 f4 f5 rest <out
case $f1 in
"["[0-9]*"]") ;;
*) pc_fail "first field is '$f1', wanted [n]" ;;
esac
[ "$f2" = + ] || pc_fail "second field is '$f2', wanted +"
[ "$f3" = "$p" ] || pc_fail "third field is '$f3', wanted the process group ID $p"
[ "$f4" = Running ] || pc_fail "fourth field is '$f4', wanted Running"
[ "$f5" = sleep ] || pc_fail "fifth field is '$f5', wanted the command"
pc_pass
