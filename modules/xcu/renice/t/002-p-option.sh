# renice -n 2 -p PID.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.renice.002
. "$PC_CORE/pc.sh"
pc_begin XCU.renice.002
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
renice -n 2 -p "$bg" >out 2>err </dev/null
rc=$?
[ "$rc" -eq 0 ] || pc_fail "renice -n 2 -p $bg exited with status $rc"
nice_of
n1=$v
renice -n 1000 -p "$bg" >/dev/null 2>&1 </dev/null
nice_of
lim=$v
if [ "$n1" -eq $((n0 + 2)) ]; then
	:
elif [ "$n1" -eq "$lim" ] && [ "$n1" -gt "$n0" ]; then
	:
elif [ "$n0" -eq "$lim" ]; then
	pc_unresolved "the child was at the upper limit ($lim) already"
else
	pc_fail "nice value went from $n0 to $n1 after an increment of 2 (limit $lim)"
fi
pc_pass
