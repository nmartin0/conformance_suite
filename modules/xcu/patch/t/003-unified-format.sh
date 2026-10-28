# patch applies a unified-context difference listing, whether named with -u or recognised, with several hunks and with line numbers that are somewhat off.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.patch.003
. "$PC_CORE/pc.sh"
pc_begin XCU.patch.003
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
	printf -- '--- t.txt%s2020-01-02 03:04:05.000000000 +0000\n' "$tab"
	printf '+++ t.txt%s2021-03-04 05:06:07.000000000 +0000\n' "$tab"
	printf '@@ -1,5 +1,5 @@\n a\n b\n-c\n+X\n d\n e\n'
} >puni || pc_unresolved "cannot create patch"
{
	printf -- '--- t.txt%s2020-01-02 03:04:05.000000000 +0000\n' "$tab"
	printf '+++ t.txt%s2021-03-04 05:06:07.000000000 +0000\n' "$tab"
	printf '@@ -1,3 +1,3 @@\n a\n-b\n+Q\n c\n@@ -4,2 +4,3 @@\n d\n e\n+F\n'
} >puni2 || pc_unresolved "cannot create patch"
{
	printf -- '--- t.txt%s2020-01-02 03:04:05.000000000 +0000\n' "$tab"
	printf '+++ t.txt%s2021-03-04 05:06:07.000000000 +0000\n' "$tab"
	printf '@@ -1,5 +1,5 @@\n a\n b\n-c\n+X\n d\n e\n'
} >pshift || pc_unresolved "cannot create patch"
pc_test
base
patch -u -i puni t.txt >/dev/null 2>&1 </dev/null
[ $? -eq 0 ] || pc_fail "-u: status not 0"
want5 t.txt a b X d e || pc_fail "-u change: the file is not a b X d e"
base
patch -i puni t.txt >/dev/null 2>&1 </dev/null
[ $? -eq 0 ] || pc_fail "auto-detected unified: status not 0"
want5 t.txt a b X d e || pc_fail "auto-detected unified: the file is not a b X d e"
base
patch -u -i puni2 t.txt >/dev/null 2>&1 </dev/null
[ $? -eq 0 ] || pc_fail "-u with two hunks: status not 0"
want5 t.txt a Q c d e F || pc_fail "-u with two hunks: the file is not a Q c d e F"
printf 'z1\nz2\na\nb\nc\nd\ne\n' >t.txt || pc_unresolved "cannot create input"
patch -u -i pshift t.txt >/dev/null 2>&1 </dev/null
[ $? -eq 0 ] || pc_fail "-u with line numbers two lines off: status not 0"
want5 t.txt z1 z2 a b X d e || pc_fail "-u with line numbers two lines off: the hunk was not applied at the matching place"
base
patch -u t.txt <puni >/dev/null 2>&1
want5 t.txt a b X d e || pc_fail "-u from standard input: the file is not a b X d e"
pc_pass
