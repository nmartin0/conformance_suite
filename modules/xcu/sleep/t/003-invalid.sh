# A bad time operand is an error.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.sleep.003
. "$PC_CORE/pc.sh"
pc_begin XCU.sleep.003
pc_test
for bad in abc 1x x1 ; do
	sleep "$bad" </dev/null >/dev/null 2>&1
	rc=$?
	[ "$rc" -gt 0 ] || pc_fail "sleep $bad exited zero"
done
pc_pass
