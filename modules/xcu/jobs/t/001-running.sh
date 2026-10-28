# jobs lists a running job in the page's format.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.jobs.001
. "$PC_CORE/pc.sh"
pc_begin XCU.jobs.001
p=
trap 'kill -s KILL "$p" 2>/dev/null' EXIT
set -m || pc_fail "set -m is not accepted"
pc_test
sleep 101 &
p=$!
jobs >out 2>/dev/null
rc=$?
[ "$rc" -eq 0 ] || pc_fail "jobs exited $rc"
{
	IFS= read -r l1
	IFS= read -r l2
} <out
case $l1 in
"["[0-9]*"] + Running sleep 101"*) ;;
*) pc_fail "jobs wrote '$l1', wanted a line [n] + Running sleep 101" ;;
esac
n=${l1#\[}
n=${n%%\]*}
case $n in
''|*[!0-9]*) pc_fail "job number '$n' is not a decimal integer" ;;
esac
[ -z "$l2" ] || pc_fail "jobs wrote a second line '$l2' for one job"
pc_pass
