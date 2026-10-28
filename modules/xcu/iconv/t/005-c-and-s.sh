# -c and -s leave the exit status alone; -s is quiet.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.iconv.005
. "$PC_CORE/pc.sh"
pc_begin XCU.iconv.005
LC_ALL=C; export LC_ALL
iconv -l >list 2>/dev/null </dev/null || pc_fail "iconv -l failed"
printf 'abc\n' >in
code=
tries=0
set -f
for w in $(cat list); do
	# the list format is unspecified: drop a trailing comma or a suffix after a slash
	w=${w%,}
	w=${w%%/*}
	case $w in
	'' | -*) continue ;;
	esac
	tries=$((tries + 1))
	[ "$tries" -le 80 ] || break
	if iconv -f "$w" -t "$w" <in >try 2>/dev/null && cmp -s in try; then
		code=$w
		break
	fi
done
[ -n "$code" ] || pc_unresolved "setup: none of the first names listed by iconv -l converts the plain text to itself"
printf 'ab\377cd\n' >bad
pc_test
iconv -f "$code" -t "$code" <bad >o0 2>/dev/null
r0=$?
iconv -c -f "$code" -t "$code" <bad >o1 2>/dev/null
r1=$?
iconv -s -f "$code" -t "$code" <bad >o2 2>e2
r2=$?
iconv -cs -f "$code" -t "$code" <bad >o3 2>e3
r3=$?
[ "$r0" -eq "$r1" ] || pc_fail "-c changed the exit status from $r0 to $r1"
[ "$r0" -eq "$r2" ] || pc_fail "-s changed the exit status from $r0 to $r2"
[ "$r0" -eq "$r3" ] || pc_fail "-cs changed the exit status from $r0 to $r3"
[ -s e2 ] && pc_fail "-s did not suppress messages about invalid characters"
[ -s e3 ] && pc_fail "-cs did not suppress messages about invalid characters"
pc_pass
