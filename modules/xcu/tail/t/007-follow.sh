# tail -f on a regular file keeps running and copies data appended to the file.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.tail.007
. "$PC_CORE/pc.sh"
pc_begin XCU.tail.007
LC_ALL=POSIX
export LC_ALL
NL='
'
printf '1\n2\n3\n' >grow || pc_fail "cannot create input"
pid=
trap '[ -n "$pid" ] && { kill "$pid" 2>/dev/null; wait "$pid" 2>/dev/null; }' EXIT
pc_test
tail -f -n 2 grow >out 2>/dev/null </dev/null &
pid=$!
i=0
while [ $i -lt 30 ]; do
	got=$(cat out; printf x)
	[ "$got" = "2${NL}3${NL}x" ] && break
	kill -0 "$pid" 2>/dev/null || pc_fail "tail -f ended before the end of the file was copied"
	sleep 1
	i=$((i + 1))
done
[ "$got" = "2${NL}3${NL}x" ] || pc_unresolved "tail -f wrote no initial output within 30 seconds"
kill -0 "$pid" 2>/dev/null || pc_fail "tail -f terminated after copying the last lines"
printf '4\n5\n' >>grow
i=0
while [ $i -lt 30 ]; do
	got=$(cat out; printf x)
	[ "$got" = "2${NL}3${NL}4${NL}5${NL}x" ] && break
	kill -0 "$pid" 2>/dev/null || pc_fail "tail -f ended before the appended data was copied"
	sleep 1
	i=$((i + 1))
done
case $got in
"2${NL}3${NL}4${NL}5${NL}x") ;;
"2${NL}3${NL}x") pc_unresolved "appended data not copied within 30 seconds (no time bound is stated)" ;;
*) pc_fail "tail -f wrote the wrong data" ;;
esac
pc_pass
