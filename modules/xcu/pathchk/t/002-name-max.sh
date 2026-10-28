# By default pathchk diagnoses a component longer than the NAME_MAX of its directory and accepts one of exactly that length.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.pathchk.002
. "$PC_CORE/pc.sh"
pc_begin XCU.pathchk.002
grow() {
	g=a
	while [ "${#g}" -le "$1" ]; do
		g=$g$g
	done
	while [ "${#g}" -gt "$1" ]; do
		g=${g%?}
	done
}

max=$(getconf NAME_MAX .) || pc_fail "getconf NAME_MAX failed"
case $max in
''|*[!0-9]*) pc_unresolved "setup: NAME_MAX is '$max'" ;;
esac
pc_test
grow "$((max + 1))"
pathchk "$g" >out 2>err </dev/null
rc=$?
[ "$rc" -ne 0 ] || pc_fail "pathchk exited 0 for a component of $((max + 1)) bytes"
[ -s err ] || pc_fail "no diagnostic"
grow "$max"
pathchk "$g" >out 2>err </dev/null
rc=$?
[ "$rc" -eq 0 ] || pc_fail "pathchk exited with status $rc for a component of $max bytes"
pc_pass
