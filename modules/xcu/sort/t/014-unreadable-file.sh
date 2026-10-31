# A file operand that cannot be read gives a status above 1.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.sort.014
# MUTANT    sort.unreadable_status0
# MUTANT    sort.unreadable_status1
# MUTANT    sort.unreadable_partial
# VARIANT   sort.status2
# VARIANT   sort.status255
# VARIANT   sort.stop_early
. "$PC_CORE/pc.sh"
pc_begin XCU.sort.014
LC_ALL=POSIX
export LC_ALL
NL='
'
[ ! -e nosuchfile ] || pc_unresolved "nosuchfile exists"
pc_test
sort nosuchfile >out 2>/dev/null </dev/null
rc=$?
[ "$rc" -gt 1 ] || pc_fail "status $rc for an unreadable file, expected above 1"
printf 'b\na\n' >good || pc_unresolved "cannot create input"
sort good nosuchfile >out 2>/dev/null </dev/null
rc=$?
[ "$rc" -gt 1 ] || pc_fail "status $rc for a readable and an unreadable file, expected above 1"
sort nosuchfile good >out 2>/dev/null </dev/null
rc=$?
[ "$rc" -gt 1 ] || pc_fail "status $rc for an unreadable and a readable file, expected above 1"
pc_pass
