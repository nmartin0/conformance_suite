# Several operands and -.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.iconv.003
. "$PC_CORE/pc.sh"
pc_begin XCU.iconv.003
LC_ALL=C; export LC_ALL
nl='
'
iconv -l >list 2>/dev/null </dev/null || pc_fail "iconv -l failed"
printf 'abc\nxyz 123\n' >in
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
printf 'one\n' >f1
printf 'three\n' >f2
printf 'two\n' >stdin.txt
pc_test
iconv -f "$code" -t "$code" f1 f2 >out 2>/dev/null </dev/null || pc_fail "iconv with two files failed"
v=$(cat out; printf x)
[ "$v" = "one${nl}three${nl}x" ] || pc_fail "two file operands gave '${v%x}'"
iconv -f "$code" -t "$code" f1 - f2 >out 2>/dev/null <stdin.txt || pc_fail "iconv with - failed"
v=$(cat out; printf x)
[ "$v" = "one${nl}two${nl}three${nl}x" ] || pc_fail "f1, - and f2 gave '${v%x}'"
iconv -f "$code" -t "$code" >out 2>/dev/null <stdin.txt || pc_fail "iconv from standard input failed"
v=$(cat out; printf x)
[ "$v" = "two${nl}x" ] || pc_fail "standard input gave '${v%x}'"
pc_pass
