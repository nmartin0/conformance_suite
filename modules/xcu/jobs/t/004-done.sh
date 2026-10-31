# A finished job is reported as Done once.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.jobs.004
. "$PC_CORE/pc.sh"
pc_begin XCU.jobs.004
set -m || pc_fail "set -m is not accepted"
pc_test
{
	true &
	sleep 1
	jobs >out 2>/dev/null
	rc=$?
	jobs >out2 2>/dev/null
} 2>shellerr
[ "$rc" -eq 0 ] || pc_fail "jobs exited $rc"
IFS= read -r line <out
lines=$(printf '%s\n' "$line" | tr -s ' ')
# a state padded to a column (more than one blank before the command) is
# accepted (Q-215). Whether a job that has already ended is still the
# current job (+) is not stated, so a <space> in that field is accepted too.
case $lines in
"["[0-9]*"] + Done true"*) ;;
"["[0-9]*"] Done true"*) ;;
*)
	if [ -z "$line" ]; then
		case $(cat shellerr) in
		*Done*) pc_unresolved "the shell reported the finished job on standard error before jobs ran" ;;
		esac
	fi
	pc_fail "jobs wrote '$line', wanted a line [n] + Done true"
	;;
esac
[ ! -s out2 ] || pc_fail "the job was listed again after it had been reported"
pc_pass
