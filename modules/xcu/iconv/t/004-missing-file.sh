# A file operand that does not exist.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.iconv.004
. "$PC_CORE/pc.sh"
pc_begin XCU.iconv.004
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
[ -e pciconv_missing.$$ ] && pc_fail "setup: the missing file exists"
pc_test
iconv -f "$code" -t "$code" pciconv_missing.$$ >out 2>err </dev/null
rc=$?
[ "$rc" -gt 0 ] || pc_fail "iconv of a nonexistent file exited with status 0"
pc_pass
