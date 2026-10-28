# A file operand that cannot be read gives a nonzero status.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.join.010
. "$PC_CORE/pc.sh"
pc_begin XCU.join.010
LC_ALL=POSIX
export LC_ALL
NL='
'
printf 'a 1\n' >f1 || pc_fail "cannot create input"
[ ! -e nosuchfile ] || pc_unresolved "nosuchfile exists"
pc_test
join f1 nosuchfile >out 2>/dev/null </dev/null
rc=$?
[ "$rc" -ne 0 ] || pc_fail "status 0 although file2 could not be read"
join nosuchfile f1 >out 2>/dev/null </dev/null
rc=$?
[ "$rc" -ne 0 ] || pc_fail "status 0 although file1 could not be read"
pc_pass
