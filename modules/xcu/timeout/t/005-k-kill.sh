# -k sends SIGKILL to a utility that ignores the first signal.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.timeout.005
. "$PC_CORE/pc.sh"
pc_begin XCU.timeout.005
pc_test
timeout -k 1 2 "${PC_SH:-sh}" -c 'trap "" TERM; n=0; while [ $n -lt 20 ]; do sleep 1; n=$((n+1)); done; echo survived' </dev/null >out 2>/dev/null
rc=$?
[ ! -s out ] || pc_fail "the utility ignoring SIGTERM was left to run on after -k time"
case $rc in
124) ;;
*)
	# or as if -p: the status of a process ended by SIGKILL
	[ "$rc" -gt 128 ] || pc_fail "timeout -k exited $rc, wanted 124 or the status of a process killed by a signal"
	;;
esac
pc_pass
