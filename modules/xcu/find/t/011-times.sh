# -mtime, -atime and -ctime compare the age in whole days: 0 is less than a day, +n more than n days, -n fewer.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.find.011
. "$PC_CORE/pc.sh"
pc_begin XCU.find.011
same_set() {
	s_a=$(printf '%s\n' $1 | sort)
	s_b=$(printf '%s\n' $2 | sort)
	[ "$s_a" = "$s_b" ]
}

mkdir tm || pc_fail "cannot create tm"
touch -t 200101010101 tm/old || pc_fail "cannot create tm/old"
: >tm/fresh || pc_fail "cannot create tm/fresh"
pc_test
got=$(find tm -type f -mtime +365 2>err </dev/null)
rc=$?
[ "$rc" -eq 0 ] || pc_fail "-mtime +365: find exited with status $rc"
same_set "$got" "tm/old" || pc_fail "-mtime +365: found '$got'"
got=$(find tm -type f -mtime 0 2>err </dev/null)
rc=$?
[ "$rc" -eq 0 ] || pc_fail "-mtime 0: find exited with status $rc"
same_set "$got" "tm/fresh" || pc_fail "-mtime 0: found '$got'"
got=$(find tm -type f -mtime -1 2>err </dev/null)
rc=$?
[ "$rc" -eq 0 ] || pc_fail "-mtime -1: find exited with status $rc"
same_set "$got" "tm/fresh" || pc_fail "-mtime -1: found '$got'"
got=$(find tm -type f -atime +365 2>err </dev/null)
rc=$?
[ "$rc" -eq 0 ] || pc_fail "-atime +365: find exited with status $rc"
same_set "$got" "tm/old" || pc_fail "-atime +365: found '$got'"
got=$(find tm -type f -atime 0 2>err </dev/null)
rc=$?
[ "$rc" -eq 0 ] || pc_fail "-atime 0: find exited with status $rc"
same_set "$got" "tm/fresh" || pc_fail "-atime 0: found '$got'"
got=$(find tm -type f -ctime 0 2>err </dev/null)
rc=$?
[ "$rc" -eq 0 ] || pc_fail "-ctime 0: find exited with status $rc"
same_set "$got" "tm/old tm/fresh" || pc_fail "-ctime 0: found '$got'"
got=$(find tm -type f -ctime +365 2>err </dev/null)
rc=$?
[ "$rc" -eq 0 ] || pc_fail "-ctime +365: find exited with status $rc"
same_set "$got" "" || pc_fail "-ctime +365: found '$got'"
pc_pass
