# nice runs its utility and passes its status.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.nice.001
. "$PC_CORE/pc.sh"
pc_begin XCU.nice.001
nl='
'
# nv COMMAND...: the nice value of the process running COMMAND (a sh -c string
# using $$); sets nvv.
isint() {
	case $1 in
	''|-|*[!0-9-]*|?*-*) return 1 ;;
	esac
	return 0
}
pc_test
nice "${PC_SH:-sh}" -c 'echo "$1 $2"; exit 3' sh x 'y z' </dev/null >out 2>/dev/null
rc=$?
[ "$rc" -eq 3 ] || pc_fail "nice exited $rc, wanted the utility's status 3"
got=$(cat out)
[ "$got" = "x y z" ] || pc_fail "the utility wrote '$got', wanted 'x y z'"
nice true </dev/null >/dev/null 2>&1
rc=$?
[ "$rc" -eq 0 ] || pc_fail "nice true exited $rc"
nice false </dev/null >/dev/null 2>&1
rc=$?
[ "$rc" -eq 1 ] || pc_fail "nice false exited $rc, wanted 1"
(exec nice "${PC_SH:-sh}" -c 'exit 4') </dev/null >/dev/null 2>&1
rc=$?
[ "$rc" -eq 4 ] || pc_fail "exec nice sh -c 'exit 4' exited $rc, wanted 4"
pc_pass
