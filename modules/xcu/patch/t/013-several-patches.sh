# A patch file holding patches for several files patches each of them, named by the headers or by Index: lines.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.patch.013
. "$PC_CORE/pc.sh"
pc_begin XCU.patch.013
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
	printf -- '--- t1.txt%s2020-01-02 03:04:05.000000000 +0000\n' "$tab"
	printf '+++ t1.txt%s2021-03-04 05:06:07.000000000 +0000\n' "$tab"
	printf '@@ -1,5 +1,5 @@\n a\n b\n-c\n+X\n d\n e\n'
	printf -- '--- t2.txt%s2020-01-02 03:04:05.000000000 +0000\n' "$tab"
	printf '+++ t2.txt%s2021-03-04 05:06:07.000000000 +0000\n' "$tab"
	printf '@@ -1,5 +1,5 @@\n a\n-b\n+Y\n c\n d\n e\n'
} >pmulti || pc_unresolved "cannot create patch"
{
	printf 'Index: t1.txt\n3c3\n< c\n---\n> X\n'
	printf 'Index: t2.txt\n2c2\n< b\n---\n> Y\n'
} >pmulti2 || pc_unresolved "cannot create patch"
pc_test
base t1.txt
base t2.txt
patch -i pmulti </dev/null >/dev/null 2>&1
rc=$?
[ "$rc" -eq 0 ] || pc_fail "a patch file for two files: status $rc, not 0"
want5 t1.txt a b X d e || pc_fail "the first file of two was not patched"
want5 t2.txt a Y c d e || pc_fail "the second file of two was not patched"
base t1.txt
base t2.txt
patch -i pmulti2 </dev/null >/dev/null 2>&1
rc=$?
[ "$rc" -eq 0 ] || pc_fail "normal patches for two files named by Index: lines: status $rc, not 0"
want5 t1.txt a b X d e || pc_fail "Index: the first file was not patched"
want5 t2.txt a Y c d e || pc_fail "Index: the second file was not patched"
base t1.txt
base t2.txt
rm -f t1.txt.orig t2.txt.orig
patch -b -i pmulti </dev/null >/dev/null 2>&1
want5 t1.txt.orig a b c d e || pc_fail "-b: t1.txt.orig missing or wrong"
want5 t2.txt.orig a b c d e || pc_fail "-b: t2.txt.orig missing or wrong"
pc_pass
