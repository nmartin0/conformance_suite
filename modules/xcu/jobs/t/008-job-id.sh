# A job_id operand limits the report to that job.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.jobs.008
. "$PC_CORE/pc.sh"
pc_begin XCU.jobs.008
p= q=
trap 'kill -s KILL "$p" "$q" 2>/dev/null' EXIT
set -m || pc_fail "set -m is not accepted"
pc_test
sleep 101 &
p=$!
sleep 102 &
q=$!
jobs '%?102' >out 2>/dev/null
rc=$?
[ "$rc" -eq 0 ] || pc_fail "jobs %?102 exited $rc"
n=0
while IFS= read -r line; do
	n=$((n + 1))
	case $line in
	*"sleep 102"*) ;;
	*) pc_fail "jobs %?102 wrote '$line'" ;;
	esac
done <out
[ "$n" -eq 1 ] || pc_fail "jobs %?102 wrote $n lines, wanted 1"
pc_pass
