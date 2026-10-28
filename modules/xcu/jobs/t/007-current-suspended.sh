# The current job is a suspended one when one exists.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.jobs.007
. "$PC_CORE/pc.sh"
pc_begin XCU.jobs.007
p= q=
trap 'kill -s KILL "$p" "$q" 2>/dev/null' EXIT
set -m || pc_fail "set -m is not accepted"
pc_test
sleep 101 &
p=$!
kill -s TSTP "$p" || pc_fail "kill -s TSTP failed"
sleep 1
sleep 102 &
q=$!
jobs >out 2>/dev/null
rc=$?
[ "$rc" -eq 0 ] || pc_fail "jobs exited $rc"
sawa= sawb=
while IFS= read -r line; do
	case $line in
	"["*"] + "*"sleep 101"*) sawa=plus ;;
	"["*"] "*"sleep 101"*) sawa=other ;;
	"["*"] + "*"sleep 102"*) sawb=plus ;;
	"["*"] "*"sleep 102"*) sawb=other ;;
	esac
done <out
[ "$sawa" = plus ] || pc_fail "the suspended job is not marked as the current job"
[ "$sawb" = other ] || pc_fail "the running job is missing from the list or also marked +"
pc_pass
