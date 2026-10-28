# -size n counts 512-byte blocks rounded up, and with the suffix c counts bytes; + and - mean more than and fewer than.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.find.010
. "$PC_CORE/pc.sh"
pc_begin XCU.find.010
same_set() {
	s_a=$(printf '%s\n' $1 | sort)
	s_b=$(printf '%s\n' $2 | sort)
	[ "$s_a" = "$s_b" ]
}

mkdir z || pc_fail "cannot create z"
: >z/z0 || pc_fail "cannot create z/z0"
printf '%01d' 0 >z/s1 || pc_fail "cannot create z/s1"
printf '%0512d' 0 >z/s512 || pc_fail "cannot create z/s512"
printf '%0513d' 0 >z/s513 || pc_fail "cannot create z/s513"
printf '%01024d' 0 >z/s1024 || pc_fail "cannot create z/s1024"
pc_test
got=$(find z -type f -size 0 2>err </dev/null)
rc=$?
[ "$rc" -eq 0 ] || pc_fail "-size 0: find exited with status $rc"
same_set "$got" "z/z0" || pc_fail "-size 0: found '$got'"
got=$(find z -type f -size 1 2>err </dev/null)
rc=$?
[ "$rc" -eq 0 ] || pc_fail "-size 1: find exited with status $rc"
same_set "$got" "z/s1 z/s512" || pc_fail "-size 1: found '$got'"
got=$(find z -type f -size 2 2>err </dev/null)
rc=$?
[ "$rc" -eq 0 ] || pc_fail "-size 2: find exited with status $rc"
same_set "$got" "z/s513 z/s1024" || pc_fail "-size 2: found '$got'"
got=$(find z -type f -size +1 2>err </dev/null)
rc=$?
[ "$rc" -eq 0 ] || pc_fail "-size +1: find exited with status $rc"
same_set "$got" "z/s513 z/s1024" || pc_fail "-size +1: found '$got'"
got=$(find z -type f -size -2 2>err </dev/null)
rc=$?
[ "$rc" -eq 0 ] || pc_fail "-size -2: find exited with status $rc"
same_set "$got" "z/z0 z/s1 z/s512" || pc_fail "-size -2: found '$got'"
got=$(find z -type f -size 513c 2>err </dev/null)
rc=$?
[ "$rc" -eq 0 ] || pc_fail "-size 513c: find exited with status $rc"
same_set "$got" "z/s513" || pc_fail "-size 513c: found '$got'"
got=$(find z -type f -size +512c 2>err </dev/null)
rc=$?
[ "$rc" -eq 0 ] || pc_fail "-size +512c: find exited with status $rc"
same_set "$got" "z/s513 z/s1024" || pc_fail "-size +512c: found '$got'"
pc_pass
