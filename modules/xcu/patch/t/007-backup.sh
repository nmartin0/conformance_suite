# patch -b saves the original in file.orig, overwriting an old one.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.patch.007
# MUTANT    patch.i_ignored
# MUTANT    patch.status1
# MUTANT    patch.last_newline_dropped
# MUTANT    patch.b_ignored
# MUTANT    patch.b_orig_is_new
# MUTANT    patch.b_orig_kept
# MUTANT    patch.b_orig_suffix
# MUTANT    patch.b_orig_each_patch
. "$PC_CORE/pc.sh"
pc_begin XCU.patch.007
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
printf '3c3\n< c\n---\n> X\n' >pchg || pc_unresolved "cannot create patch"
pc_test
base
rm -f t.txt.orig
patch -b -i pchg t.txt >/dev/null 2>&1 </dev/null
[ $? -eq 0 ] || pc_fail "-b: status not 0"
want5 t.txt a b X d e || pc_fail "-b: the file is not a b X d e"
[ -f t.txt.orig ] || pc_fail "-b did not create t.txt.orig"
want5 t.txt.orig a b c d e || pc_fail "t.txt.orig does not hold the original a b c d e"
# Without -b the text does not say whether a .orig file may be left, so
# nothing is asserted about that case.
base
printf 'stale\n' >t.txt.orig || pc_unresolved "cannot create input"
patch -b -i pchg t.txt >/dev/null 2>&1 </dev/null
want5 t.txt.orig a b c d e || pc_fail "an existing t.txt.orig was not overwritten"
base
rm -f t.txt.orig
printf '3c3\n< c\n---\n> X\n4c4\n< d\n---\n> Y\n' >ptwo || pc_unresolved "cannot create patch"
patch -b -i ptwo t.txt >/dev/null 2>&1 </dev/null
want5 t.txt a b X Y e || pc_fail "-b with two hunks: the file is not a b X Y e"
want5 t.txt.orig a b c d e || pc_fail "-b with two hunks: t.txt.orig does not hold the original"
# several separate patches for one file: the saved copy is of the original, written for the first patch only
base
rm -f t.txt.orig
printf 'Index: t.txt\n3c3\n< c\n---\n> X\nIndex: t.txt\n4c4\n< d\n---\n> Y\n' >pseq || pc_unresolved "cannot create patch"
patch -b -i pseq t.txt >/dev/null 2>&1 </dev/null
want5 t.txt a b X Y e || pc_unresolved "two patches for one file were not both applied"
want5 t.txt.orig a b c d e || pc_fail "-b with two patches for one file: t.txt.orig is not the original a b c d e"
pc_pass
