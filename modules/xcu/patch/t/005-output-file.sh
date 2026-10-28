# patch -o writes the patched file to the named file and leaves the original alone.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.patch.005
. "$PC_CORE/pc.sh"
pc_begin XCU.patch.005
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
rm -f out out.orig
patch -o out -i pchg t.txt >/dev/null 2>&1 </dev/null
[ $? -eq 0 ] || pc_fail "-o: status not 0"
want5 out a b X d e || pc_fail "-o: the output file is not a b X d e"
want5 t.txt a b c d e || pc_fail "-o: the original file was modified"
base
rm -f out out.orig
printf 'old\n' >out || pc_unresolved "cannot create input"
patch -b -o out -i pchg t.txt >/dev/null 2>&1 </dev/null
want5 out a b X d e || pc_fail "-o with an existing outfile: the output file is not a b X d e"
[ -f out.orig ] || pc_fail "-b -o with an existing outfile did not create outfile.orig"
want5 out.orig old || pc_fail "outfile.orig does not hold the old outfile"
[ ! -e t.txt.orig ] || pc_fail "-b -o created file.orig"
pc_pass
