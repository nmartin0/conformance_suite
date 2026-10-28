# A huge increment is bounded, not an error.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.renice.003
. "$PC_CORE/pc.sh"
pc_begin XCU.renice.003
LC_ALL=C; export LC_ALL
bg=
trap '[ -n "$bg" ] && { kill "$bg" 2>/dev/null; wait "$bg" 2>/dev/null; }' EXIT
sleep 301 </dev/null >/dev/null 2>&1 &
bg=$!
nice_of() {
	v=$(ps -o nice= -p "$bg") || pc_fail "ps -o nice= failed"
	set -- $v
	v=$1
	case $v in
	[0-9]* | -[0-9]*) ;;
	*) pc_unresolved "setup: nice value '$v' is not a number" ;;
	esac
}
nice_of
n0=$v
pc_test
renice -n 1000 "$bg" >out 2>err </dev/null
rc=$?
[ "$rc" -eq 0 ] || pc_fail "renice -n 1000 $bg exited with status $rc"
nice_of
n1=$v
[ "$n1" -ge "$n0" ] || pc_fail "nice value fell from $n0 to $n1"
renice -n 1000 "$bg" >out 2>err </dev/null
rc=$?
[ "$rc" -eq 0 ] || pc_fail "second renice -n 1000 exited with status $rc"
nice_of
n2=$v
[ "$n2" -eq "$n1" ] || pc_fail "the nice value moved from $n1 to $n2 at the limit"
pc_pass
