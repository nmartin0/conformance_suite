# When a file operand cannot be read, awk writes a diagnostic, terminates without further action and exits above zero.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.awk.018
. "$PC_CORE/pc.sh"
pc_begin XCU.awk.018
LC_ALL=POSIX
export LC_ALL
NL='
'
printf '1\n' >f1 || pc_fail "cannot create input"
[ ! -e nosuchfile ] || pc_unresolved "nosuchfile exists"
pc_test
out=$(awk '{ print } END { print "end" }' nosuchfile f1 2>/dev/null </dev/null)
rc=$?
[ "$rc" -ne 0 ] || pc_fail "status 0 for a missing file operand"
[ -z "$out" ] || pc_fail "awk went on after a missing file operand"
err=$(awk '{ print }' nosuchfile 2>&1 >/dev/null </dev/null)
[ -n "$err" ] || pc_fail "no diagnostic for a missing file operand"
awk 'BEGIN { x = 1 }' >/dev/null 2>&1 </dev/null
[ $? -eq 0 ] || pc_fail "a successful run did not give status 0"
awk '{ print }' f1 >/dev/null 2>&1 </dev/null
[ $? -eq 0 ] || pc_fail "reading a file successfully did not give status 0"
pc_pass
