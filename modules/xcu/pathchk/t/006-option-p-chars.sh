# With -p, characters outside the portable filename character set are diagnosed.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.pathchk.006
. "$PC_CORE/pc.sh"
pc_begin XCU.pathchk.006
pc_test
pathchk -p 'a.B_c-9/d' >out 2>err </dev/null
rc=$?
[ "$rc" -eq 0 ] || pc_fail "pathchk -p exited with status $rc for a portable name"
for bad in 'a b' 'a*b' 'a@b' 'a$b' 'a:b'; do
	pathchk -p "$bad" >out 2>err </dev/null
	rc=$?
	[ "$rc" -ne 0 ] || pc_fail "pathchk -p exited 0 for '$bad'"
	[ -s err ] || pc_fail "no diagnostic for '$bad'"
done
pc_pass
