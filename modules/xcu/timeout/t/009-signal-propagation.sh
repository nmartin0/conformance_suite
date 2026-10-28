# A signal that ends the utility ends timeout the same way.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.timeout.009
. "$PC_CORE/pc.sh"
pc_begin XCU.timeout.009
{ "${PC_SH:-sh}" -c 'kill -s USR1 $$' </dev/null >/dev/null 2>&1; } 2>/dev/null
plain=$?
[ "$plain" -gt 128 ] || pc_unresolved "setup: a shell that signals itself with USR1 gave status $plain"
pc_test
{ timeout 30 "${PC_SH:-sh}" -c 'kill -s USR1 $$' </dev/null >/dev/null 2>&1; } 2>/dev/null
rc=$?
[ "$rc" -eq "$plain" ] || pc_fail "timeout exited $rc, wanted $plain (the status of a process ended by the same signal)"
pc_pass
