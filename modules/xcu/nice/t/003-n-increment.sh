# nice -n 1 adds one to the nice value.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.nice.003
. "$PC_CORE/pc.sh"
pc_begin XCU.nice.003
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
mine=$(ps -o nice= -p $$ 2>/dev/null)
mine=$(echo $mine)
isint "$mine" || pc_unresolved "setup: ps -o nice= -p gave '$mine'"
# The nice values run from 0 to 2*NZERO-1 (the value ps shows may also be
# counted from NZERO, so the top may be NZERO-1): the increment cannot be seen
# when the caller already sits at a top.
nz=$(getconf NZERO 2>/dev/null </dev/null)
case $nz in
''|*[!0-9]*) ;;
*)
	if [ "$mine" -eq $((2 * nz - 1)) ] || [ "$mine" -eq $((nz - 1)) ]; then
		pc_unresolved "setup: the caller's nice value $mine is at the highest, so it cannot be raised by one"
	fi
	;;
esac
pc_test
nice -n 1 "${PC_SH:-sh}" -c 'ps -o nice= -p $$' </dev/null >out 2>/dev/null
rc=$?
[ "$rc" -eq 0 ] || pc_fail "nice -n 1 exited $rc"
child=$(cat out)
child=$(echo $child)
isint "$child" || pc_unresolved "ps gave '$child' for the process run by nice"
want=$((mine + 1))
[ "$child" -eq "$want" ] || pc_fail "the utility has nice value $child, wanted $want (caller $mine plus one)"
pc_pass
