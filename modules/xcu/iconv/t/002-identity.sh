# Same-codeset conversion is the identity.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.iconv.002
. "$PC_CORE/pc.sh"
pc_begin XCU.iconv.002
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
pc_test
iconv -f "$code" -t "$code" <in >out 2>err || pc_fail "iconv -f $code -t $code failed"
a=$(cat in; printf x)
b=$(cat out; printf x)
[ "$a" = "$b" ] || pc_fail "converting from $code to $code changed the text: '${b%x}'"
[ -s err ] && pc_fail "iconv wrote to standard error for a valid text"
iconv -f "$code" -t "$code" in >out 2>err || pc_fail "iconv with a file operand failed"
b=$(cat out; printf x)
[ "$a" = "$b" ] || pc_fail "converting a file operand changed the text: '${b%x}'"
pc_pass
