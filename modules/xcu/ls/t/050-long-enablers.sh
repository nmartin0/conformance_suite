# ls -n with -C, -m and -x, and -1 with -l: the last of the long format switches decides
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.ls.050
. "$PC_CORE/pc.sh"
pc_begin XCU.ls.050
LC_ALL=POSIX; export LC_ALL
mkdir d || pc_unresolved "cannot create d"
: >d/a || pc_unresolved "cannot create d/a"
pc_test
# long <options...>: succeed when ls on d wrote the long format (a total line)
long() {
	ls "$@" d >out 2>err </dev/null || pc_fail "ls $* d failed"
	grep '^total ' out >/dev/null
}
for o in -C -m -x; do
	long -n $o && pc_fail "ls -n $o still wrote the long format"
	long $o -n || pc_fail "ls $o -n did not write the long format"
done
long -l -1 || pc_fail "ls -l -1 did not write the long format: -1 must not disable it"
long -1 -l || pc_fail "ls -1 -l did not write the long format"
long -n -1 || pc_fail "ls -n -1 did not write the long format"
pc_pass
