# Status 0 and a quiet standard error.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.uname.006
. "$PC_CORE/pc.sh"
pc_begin XCU.uname.006
LC_ALL=C; export LC_ALL
pc_test
for o in "" -a -m -n -r -s -v -mnrsv; do
	uname $o >out 2>err </dev/null
	rc=$?
	[ "$rc" -eq 0 ] || pc_fail "uname $o exited with status $rc"
	[ -s err ] && pc_fail "uname $o wrote to standard error"
done
pc_pass
