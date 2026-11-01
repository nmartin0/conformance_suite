# patch finds the file to patch from the operand, the headers or an Index: line, strips leading components with -p, and works in the directory named by -d.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.patch.008
# MUTANT    patch.p_ignored
# MUTANT    patch.p_one_more
# MUTANT    patch.p_one_less
# MUTANT    patch.p_zero_as_basename
# MUTANT    patch.basename_not_used
# MUTANT    patch.d_ignored
# MUTANT    patch.index_ignored
# MUTANT    patch.operand_ignored
# MUTANT    patch.slash_not_component
# MUTANT    patch.second_name_unused
. "$PC_CORE/pc.sh"
pc_begin XCU.patch.008
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
P=$PWD
{
	printf -- '--- a/dir/t.txt%s2020-01-02 03:04:05.000000000 +0000\n' "$tab"
	printf '+++ b/dir/t.txt%s2021-03-04 05:06:07.000000000 +0000\n' "$tab"
	printf '@@ -1,5 +1,5 @@\n a\n b\n-c\n+X\n d\n e\n'
} >ppath || pc_unresolved "cannot create patch"
{
	printf 'Index: t.txt\n'
	printf '3c3\n< c\n---\n> X\n'
} >pindex || pc_unresolved "cannot create patch"
printf '3c3\n< c\n---\n> X\n' >pchg || pc_unresolved "cannot create patch"
{
	printf -- '--- /dir/t.txt%s2020-01-02 03:04:05.000000000 +0000\n' "$tab"
	printf '+++ /dir/t.txt%s2021-03-04 05:06:07.000000000 +0000\n' "$tab"
	printf '@@ -1,5 +1,5 @@\n a\n b\n-c\n+X\n d\n e\n'
} >pabs || pc_unresolved "cannot create patch"
{
	printf -- '--- a/b/dir/t.txt%s2020-01-02 03:04:05.000000000 +0000\n' "$tab"
	printf '+++ a/b/dir/t.txt%s2021-03-04 05:06:07.000000000 +0000\n' "$tab"
	printf '@@ -1,5 +1,5 @@\n a\n b\n-c\n+X\n d\n e\n'
} >ppath2 || pc_unresolved "cannot create patch"
{
	printf -- '--- nosuch.txt%s2020-01-02 03:04:05.000000000 +0000\n' "$tab"
	printf '+++ t.txt%s2021-03-04 05:06:07.000000000 +0000\n' "$tab"
	printf '@@ -1,5 +1,5 @@\n a\n b\n-c\n+X\n d\n e\n'
} >pnew || pc_unresolved "cannot create patch"
mkdir s7 s7/dir s8 s8/dir s9 || pc_unresolved "cannot create directories"
base s7/dir/t.txt
base s7/t.txt
base s8/dir/t.txt
base s8/t.txt
base s9/t.txt
mkdir s1 s1/dir s2 s2/a s2/a/dir s3 s3/dir s4 s5 s5/other s6 || pc_unresolved "cannot create directories"
base s1/dir/t.txt
base s2/a/dir/t.txt
base s3/t.txt
base s3/dir/t.txt
base s4/t.txt
base s5/other/t.txt
base s5/t.txt
base s6/t.txt
pc_test
(cd s1 && patch -p 1 -i "$P/ppath" >/dev/null 2>&1 </dev/null)
want5 s1/dir/t.txt a b X d e || pc_fail "-p 1 did not make a/dir/t.txt mean dir/t.txt"
(cd s2 && patch -p 0 -i "$P/ppath" >/dev/null 2>&1 </dev/null)
want5 s2/a/dir/t.txt a b X d e || pc_fail "-p 0 did not use the full pathname a/dir/t.txt"
(cd s3 && patch -i "$P/ppath" >/dev/null 2>&1 </dev/null)
want5 s3/t.txt a b X d e || pc_fail "without -p, the base name t.txt was not used"
want5 s3/dir/t.txt a b c d e || pc_fail "without -p, dir/t.txt was changed"
(cd s4 && patch -i "$P/pindex" >/dev/null 2>&1 </dev/null)
want5 s4/t.txt a b X d e || pc_fail "the Index: line did not name the file"
(cd s5 && patch -d other -i "$P/pchg" t.txt >/dev/null 2>&1 </dev/null)
want5 s5/other/t.txt a b X d e || pc_fail "-d did not make the file operand relative to the named directory"
want5 s5/t.txt a b c d e || pc_fail "-d: the file in the current directory was changed"
(cd s6 && patch -i "$P/pchg" t.txt >/dev/null 2>&1 </dev/null)
want5 s6/t.txt a b X d e || pc_fail "a file operand was not used"
(cd s7 && patch -p 1 -i "$P/pabs" >/dev/null 2>&1 </dev/null)
want5 s7/dir/t.txt a b X d e || pc_fail "-p 1 did not count the leading slash of /dir/t.txt as the first component"
want5 s7/t.txt a b c d e || pc_fail "-p 1 on /dir/t.txt changed t.txt instead of dir/t.txt"
(cd s8 && patch -p 2 -i "$P/ppath2" >/dev/null 2>&1 </dev/null)
want5 s8/dir/t.txt a b X d e || pc_fail "-p 2 did not make a/b/dir/t.txt mean dir/t.txt"
want5 s8/t.txt a b c d e || pc_fail "-p 2 on a/b/dir/t.txt changed t.txt instead of dir/t.txt"
(cd s9 && patch -i "$P/pnew" >/dev/null 2>&1 </dev/null)
want5 s9/t.txt a b X d e || pc_fail "the name on the second header line was not used when the file named on the first does not exist"
pc_pass
