# patch applies a copied-context difference listing, whether named with -c or recognised.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.patch.002
. "$PC_CORE/pc.sh"
pc_begin XCU.patch.002
LC_ALL=POSIX
export LC_ALL
NL='
'
base() { printf 'a\nb\nc\nd\ne\n' >"${1:-t.txt}" || pc_unresolved "cannot create input"; }
want5() { # $1 file, rest: expected lines
	f=$1
	shift
	exp=
	for l in "$@"; do exp=$exp$l$NL; done
	got=$(cat "$f" 2>/dev/null; printf x)
	[ "$got" = "${exp}x" ]
}
tab=$(printf '\t')
{
	printf '*** t.txt%sThu Jan  2 03:04:05 2020\n' "$tab"
	printf -- '--- t.txt%sThu Mar  4 05:06:07 2021\n' "$tab"
	printf '***************\n*** 1,5 ****\n  a\n  b\n! c\n  d\n  e\n--- 1,5 ----\n  a\n  b\n! X\n  d\n  e\n'
} >pctx || pc_unresolved "cannot create patch"
{
	printf '*** t.txt%sThu Jan  2 03:04:05 2020\n' "$tab"
	printf -- '--- t.txt%sThu Mar  4 05:06:07 2021\n' "$tab"
	printf '***************\n*** 2,4 ****\n  b\n- c\n  d\n--- 2,3 ----\n  b\n  d\n'
} >pctxd || pc_unresolved "cannot create patch"
{
	printf '*** t.txt%sThu Jan  2 03:04:05 2020\n' "$tab"
	printf -- '--- t.txt%sThu Mar  4 05:06:07 2021\n' "$tab"
	printf '***************\n*** 2,3 ****\n  b\n  c\n--- 2,4 ----\n  b\n+ N\n  c\n'
} >pctxa || pc_unresolved "cannot create patch"
pc_test
base
patch -c -i pctx t.txt >/dev/null 2>&1 </dev/null
[ $? -eq 0 ] || pc_fail "-c: status not 0"
want5 t.txt a b X d e || pc_fail "-c change: the file is not a b X d e"
base
patch -i pctx t.txt >/dev/null 2>&1 </dev/null
[ $? -eq 0 ] || pc_fail "auto-detected copied context: status not 0"
want5 t.txt a b X d e || pc_fail "auto-detected copied context: the file is not a b X d e"
base
patch -c -i pctxd t.txt >/dev/null 2>&1 </dev/null
want5 t.txt a b d e || pc_fail "-c delete: the file is not a b d e"
base
patch -c -i pctxa t.txt >/dev/null 2>&1 </dev/null
want5 t.txt a b N c d e || pc_fail "-c add: the file is not a b N c d e"
base
patch -c t.txt <pctx >/dev/null 2>&1
want5 t.txt a b X d e || pc_fail "-c from standard input: the file is not a b X d e"
pc_pass
