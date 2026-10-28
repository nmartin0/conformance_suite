# patch -b saves the original in file.orig, overwriting an old one, and without -b no .orig file is made.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.patch.007
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
want5 t.txt a b X d e || pc_fail "-b: the file is not a b X d e"
[ -f t.txt.orig ] || pc_fail "-b did not create t.txt.orig"
want5 t.txt.orig a b c d e || pc_fail "t.txt.orig does not hold the original a b c d e"
base
rm -f t.txt.orig
patch -i pchg t.txt >/dev/null 2>&1 </dev/null
[ ! -e t.txt.orig ] || pc_fail "t.txt.orig was created without -b"
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
pc_pass
