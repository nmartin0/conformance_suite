# patch -l lets any run of blanks in the patch match any run of blanks in the file, which otherwise must match exactly.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.patch.011
. "$PC_CORE/pc.sh"
pc_begin XCU.patch.011
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
printf 'a\nx  y\nc\n' >t.txt || pc_unresolved "cannot create input"
printf '2c2\n< x y\n---\n> Z\n' >pb || pc_unresolved "cannot create patch"
printf 'a\nx\t y\nc\n' >t2.txt || pc_unresolved "cannot create input"
pc_test
patch -l -i pb t.txt >/dev/null 2>&1 </dev/null
[ $? -eq 0 ] || pc_fail "-l: status not 0"
want5 t.txt a Z c || pc_fail "-l: a run of two blanks did not match one blank in the patch"
printf 'a\nx  y\nc\n' >t.txt || pc_unresolved "cannot create input"
rm -f t.txt.rej
patch -i pb t.txt >/dev/null 2>&1 </dev/null
[ $? -eq 1 ] || pc_fail "without -l, differing blank runs did not give a reject"
want5 t.txt a 'x  y' c || pc_fail "without -l, the file was changed"
patch -l -i pb t2.txt >/dev/null 2>&1 </dev/null
want5 t2.txt a Z c || pc_fail "-l: a tab and a blank did not match one blank in the patch"
pc_pass
