# A file operand that cannot be read gives a nonzero status.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.tail.006
. "$PC_CORE/pc.sh"
pc_begin XCU.tail.006
LC_ALL=POSIX
export LC_ALL
NL='
'
[ ! -e nosuchfile ] || pc_unresolved "nosuchfile exists"
pc_test
tail -n 1 nosuchfile >out 2>/dev/null </dev/null
rc=$?
[ "$rc" -ne 0 ] || pc_fail "status 0 for a file that cannot be read"
pc_pass
