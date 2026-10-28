# ls: success is status 0 with nothing on standard error
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.ls.049
. "$PC_CORE/pc.sh"
pc_begin XCU.ls.049
LC_ALL=POSIX; export LC_ALL
mkdir d || pc_unresolved "cannot create d"
: >d/a || pc_unresolved "cannot create d/a"
pc_test
for o in "" -l -R -a -F -s -i -C -m; do
	ls $o d >out 2>err </dev/null
	rc=$?
	[ "$rc" -eq 0 ] || pc_fail "ls $o d exited with status $rc"
	[ -s err ] && pc_fail "ls $o d wrote to standard error: $(cat err)"
	[ -s out ] || pc_fail "ls $o d wrote nothing to standard output"
done
pc_pass
