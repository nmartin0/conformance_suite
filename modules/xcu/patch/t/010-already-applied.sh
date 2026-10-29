# By default an already-applied patch is rejected, and -N ignores it.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.patch.010
. "$PC_CORE/pc.sh"
pc_begin XCU.patch.010
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
patch -i pchg t.txt >/dev/null 2>&1 </dev/null
[ $? -eq 0 ] || pc_unresolved "the first application of the patch failed"
rm -f t.txt.rej
patch -i pchg t.txt >/dev/null 2>&1 </dev/null
# the page lets patch ask whether to assume -R here; a "yes" would restore the original
if want5 t.txt a b X d e; then
	[ -s t.txt.rej ] || pc_fail "by default an already-applied patch was not rejected"
else
	want5 t.txt a b c d e || pc_fail "applying a patch twice left the file in a state that is neither patched nor original"
fi
base
patch -i pchg t.txt >/dev/null 2>&1 </dev/null
rm -f t.txt.rej
patch -N -i pchg t.txt >/dev/null 2>&1 </dev/null
want5 t.txt a b X d e || pc_fail "-N: the file was changed by an already-applied patch"
# Whether an ignored hunk is also recorded in a reject file is not stated
# (a reject file "may" be created), so only the file's content is checked.
base
rm -f t.txt.rej
patch -N -i pchg t.txt >/dev/null 2>&1 </dev/null
want5 t.txt a b X d e || pc_fail "-N: a patch not yet applied was not applied"
pc_pass
