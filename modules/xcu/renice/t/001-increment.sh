# renice -n 1 PID.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.renice.001
. "$PC_CORE/pc.sh"
pc_begin XCU.renice.001
LC_ALL=C; export LC_ALL
bg=
trap '[ -n "$bg" ] && { kill "$bg" 2>/dev/null; wait "$bg" 2>/dev/null; }' EXIT
sleep 301 </dev/null >/dev/null 2>&1 &
bg=$!
# nice: v = nice value of the child, as a number.
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
renice -n 1 "$bg" >out 2>err </dev/null
rc=$?
[ "$rc" -eq 0 ] || pc_fail "renice -n 1 $bg exited with status $rc"
nice_of
n1=$v
# find the limit with a huge increment
renice -n 1000 "$bg" >/dev/null 2>&1 </dev/null
nice_of
lim=$v
if [ "$n1" -eq $((n0 + 1)) ]; then
	:
elif [ "$n1" -eq "$lim" ] && [ "$n0" -eq "$lim" ]; then
	pc_unresolved "the child was at the upper limit ($lim) already"
else
	pc_fail "nice value went from $n0 to $n1 after an increment of 1 (limit $lim)"
fi
pc_pass
