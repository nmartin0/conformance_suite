# A file operand that cannot be read gives a nonzero status.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.comm.005
. "$PC_CORE/pc.sh"
pc_begin XCU.comm.005
LC_ALL=POSIX
export LC_ALL
NL='
'
printf 'a\n' >f1 || pc_fail "cannot create input"
[ ! -e nosuchfile ] || pc_unresolved "nosuchfile exists"
pc_test
comm f1 nosuchfile >out 2>/dev/null </dev/null
rc=$?
[ "$rc" -ne 0 ] || pc_fail "status 0 although file2 could not be read"
comm nosuchfile f1 >out 2>/dev/null </dev/null
rc=$?
[ "$rc" -ne 0 ] || pc_fail "status 0 although file1 could not be read"
pc_pass
