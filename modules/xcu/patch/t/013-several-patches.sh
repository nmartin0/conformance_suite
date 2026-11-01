# A patch file holding patches for several files patches each of them, named by the headers or by Index: lines.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.patch.013
# MUTANT    patch.i_ignored
# MUTANT    patch.status1
# MUTANT    patch.second_not_patched
# MUTANT    patch.first_not_patched
# MUTANT    patch.names_all_first
# MUTANT    patch.index_ignored
# MUTANT    patch.b_first_only
# MUTANT    patch.indent_not_removed
# MUTANT    patch.indent_tab_not_removed
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
{
	printf '  --- t1.txt%s2020-01-02 03:04:05.000000000 +0000\n' "$tab"
	printf '  +++ t1.txt%s2021-03-04 05:06:07.000000000 +0000\n' "$tab"
	printf '  @@ -1,5 +1,5 @@\n   a\n   b\n  -c\n  +X\n   d\n   e\n'
} >pind1 || pc_unresolved "cannot create patch"
{
	printf '%sIndex: t2.txt\n%s2c2\n%s< b\n%s---\n%s> Y\n' "$tab" "$tab" "$tab" "$tab" "$tab"
} >pind2 || pc_unresolved "cannot create patch"
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
# every line of a patch carries the same leading blanks: they are removed first
base t1.txt
patch -i pind1 </dev/null >/dev/null 2>&1
want5 t1.txt a b X d e || pc_fail "a unified patch whose lines all start with two blanks was not applied"
base t2.txt
patch -i pind2 </dev/null >/dev/null 2>&1
want5 t2.txt a Y c d e || pc_fail "a normal patch whose lines all start with a tab was not applied"
pc_pass
