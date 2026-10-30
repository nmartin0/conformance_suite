# A job stopped by SIGTSTP is reported Stopped.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.jobs.006
. "$PC_CORE/pc.sh"
pc_begin XCU.jobs.006
p=
trap 'kill -s KILL "$p" 2>/dev/null' EXIT
set -m || pc_fail "set -m is not accepted"
case $- in
*m*) ;;
*) pc_unresolved "job control could not be turned on (the shell has no controlling terminal?)" ;;
esac
pc_test
sleep 101 &
p=$!
kill -s TSTP "$p" || pc_fail "kill -s TSTP failed"
sleep 1
jobs >out 2>/dev/null
rc=$?
[ "$rc" -eq 0 ] || pc_fail "jobs exited $rc"
IFS= read -r line <out
lines=$(printf '%s\n' "$line" | tr -s ' ')
# a state padded to a column (more than one blank before the command) is
# accepted (Q-215)
case $lines in
"["[0-9]*"] + Stopped sleep 101"*) ;;
"["[0-9]*"] + Stopped (SIGTSTP) sleep 101"*) ;;
"["[0-9]*"] + Suspended sleep 101"*) ;;
"["[0-9]*"] + Suspended (SIGTSTP) sleep 101"*) ;;
*) pc_fail "jobs wrote '$line', wanted a line [n] + Stopped sleep 101" ;;
esac
pc_pass
