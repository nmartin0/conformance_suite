# By default pathchk diagnoses a pathname longer than PATH_MAX.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.pathchk.003
. "$PC_CORE/pc.sh"
pc_begin XCU.pathchk.003
max=$(getconf PATH_MAX .) || pc_fail "getconf PATH_MAX failed"
case $max in
''|*[!0-9]*) pc_unresolved "setup: PATH_MAX is '$max'" ;;
esac
p=a/
while [ "${#p}" -le "$((max + 16))" ]; do
	p=$p$p
done
pc_test
pathchk "$p" >out 2>err </dev/null
rc=$?
[ "$rc" -ne 0 ] || pc_fail "pathchk exited 0 for a pathname of ${#p} bytes"
[ -s err ] || pc_fail "no diagnostic"
pc_pass
