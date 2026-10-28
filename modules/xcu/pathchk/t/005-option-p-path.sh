# With -p, a pathname longer than 256 bytes is diagnosed and one of at most 256 bytes is accepted.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.pathchk.005
. "$PC_CORE/pc.sh"
pc_begin XCU.pathchk.005
p1=
i=0
while [ "$i" -lt 20 ]; do
	p1=${p1}abcdefghijklm/
	i=$((i + 1))
done
p2=
i=0
while [ "$i" -lt 17 ]; do
	p2=${p2}abcdefghijklm/
	i=$((i + 1))
done
p2=${p2}x
pc_test
pathchk -p "$p1" >out 2>err </dev/null
rc=$?
[ "$rc" -ne 0 ] || pc_fail "pathchk -p exited 0 for a pathname of ${#p1} bytes"
[ -s err ] || pc_fail "no diagnostic"
pathchk -p "$p2" >out 2>err </dev/null
rc=$?
[ "$rc" -eq 0 ] || pc_fail "pathchk -p exited with status $rc for a pathname of ${#p2} bytes"
pc_pass
