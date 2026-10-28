# comm and args of a sleeping child.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.ps.010
. "$PC_CORE/pc.sh"
pc_begin XCU.ps.010
LC_ALL=C; export LC_ALL
bg=
trap '[ -n "$bg" ] && { kill "$bg" 2>/dev/null; wait "$bg" 2>/dev/null; }' EXIT
sleep 301 </dev/null >/dev/null 2>&1 &
bg=$!
pc_test
# The child may not have started sleep yet; look again for a few seconds.
n=0
while :; do
	ps -o comm= -p "$bg" >out 2>err </dev/null || pc_fail "ps -o comm= failed"
	read -r c <out
	[ "$c" = sleep ] && break
	n=$((n + 1))
	[ "$n" -lt 5 ] || break
	sleep 1
done
[ "$c" = sleep ] || pc_fail "comm is '$c', expected sleep"
ps -o args= -p "$bg" >out 2>err </dev/null || pc_fail "ps -o args= failed"
read -r a <out
case $a in
sleep*) ;;
*) pc_fail "args is '$a', expected a line beginning with sleep" ;;
esac
pc_pass
