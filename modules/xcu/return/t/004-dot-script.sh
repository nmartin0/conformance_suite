# return in a file read by the dot utility stops that file; the shell
# continues after the dot command with the status given.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.return.004
. "$PC_CORE/pc.sh"
pc_begin XCU.return.004
printf '%s\n' 'echo a' 'return 5' 'echo b' >pc.r1 || pc_unresolved "write"
pc_test
. ./pc.r1 >pc.ro
rc=$?
[ "$rc" -eq 5 ] || pc_fail "status $rc, expected 5"
[ "$(cat pc.ro)" = a ] || pc_fail "file output was '$(cat pc.ro)', expected a"
pc_pass
