# ls -g and -o with -C, -m and -x, and -1: the last of the long format switches decides
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.ls.051
. "$PC_CORE/pc.sh"
pc_begin XCU.ls.051
LC_ALL=POSIX; export LC_ALL
mkdir d || pc_unresolved "cannot create d"
: >d/a || pc_unresolved "cannot create d/a"
pc_test
# long <options...>: succeed when ls on d wrote the long format (a total line)
long() {
	ls "$@" d >out 2>err </dev/null || pc_fail "ls $* d failed"
	grep '^total ' out >/dev/null
}
for e in -g -o; do
	for o in -C -m -x; do
		long $e $o && pc_fail "ls $e $o still wrote the long format"
		long $o $e || pc_fail "ls $o $e did not write the long format"
	done
	long $e -1 || pc_fail "ls $e -1 did not write the long format"
	long -1 $e || pc_fail "ls -1 $e did not write the long format"
done
pc_pass
