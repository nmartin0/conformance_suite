# A hunk that cannot be placed goes to the reject file (t.txt.rej or the -r name) in copied-context form and the status is 1; other hunks are still applied.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.patch.009
. "$PC_CORE/pc.sh"
pc_begin XCU.patch.009
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
printf '3c3\n< Q\n---\n> X\n' >pbad || pc_unresolved "cannot create patch"
printf '3c3\n< c\n---\n> X\n' >pchg || pc_unresolved "cannot create patch"
pc_test
base
rm -f t.txt.rej rej.out
patch -i pbad t.txt >/dev/null 2>&1 </dev/null
rc=$?
[ "$rc" -eq 1 ] || pc_fail "a hunk that does not fit gave status $rc, not 1"
want5 t.txt a b c d e || pc_fail "the file was changed by a hunk that does not fit"
[ -s t.txt.rej ] || pc_fail "no reject file t.txt.rej was written"
if [ -s t.txt.rej ]; then
	hasstar=0
	hasdash=0
	while IFS= read -r l; do
		case $l in
		'*** '*) hasstar=1 ;;
		'--- '*) hasdash=1 ;;
		esac
	done <t.txt.rej
	[ $hasstar -eq 1 ] && [ $hasdash -eq 1 ] || pc_fail "the reject file of a normal-format hunk is not in copied-context format"
	case $(cat t.txt.rej) in
	*Q*) ;;
	*) pc_fail "the reject file does not show the rejected hunk" ;;
	esac
fi
base
rm -f t.txt.rej rej.out
patch -r rej.out -i pbad t.txt >/dev/null 2>&1 </dev/null
[ $? -eq 1 ] || pc_fail "-r: status not 1 for a rejected hunk"
[ -s rej.out ] || pc_fail "-r did not write the named reject file"
[ ! -e t.txt.rej ] || pc_fail "-r: the default reject file was written too"
base
rm -f t.txt.rej
printf '3c3\n< c\n---\n> X\n4c4\n< Q\n---\n> Y\n' >phalf || pc_unresolved "cannot create patch"
patch -i phalf t.txt >/dev/null 2>&1 </dev/null
rc=$?
[ "$rc" -eq 1 ] || pc_fail "one good and one bad hunk gave status $rc, not 1"
want5 t.txt a b X d e || pc_fail "the good hunk was not applied when the other was rejected"
[ -s t.txt.rej ] || pc_fail "the bad hunk was not written to a reject file"
base
rm -f t.txt.rej
patch -i pchg t.txt >/dev/null 2>&1 </dev/null
[ $? -eq 0 ] || pc_fail "a good patch did not give status 0"
[ ! -e t.txt.rej ] || pc_fail "a reject file was written for a good patch"
pc_pass
