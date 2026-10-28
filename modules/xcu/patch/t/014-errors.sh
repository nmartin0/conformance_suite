# patch exits above 1 when the patch file or the file to patch cannot be used.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.patch.014
. "$PC_CORE/pc.sh"
pc_begin XCU.patch.014
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
[ ! -e nosuchfile ] || pc_unresolved "nosuchfile exists"
pc_test
base
patch -i nosuchfile t.txt >/dev/null 2>&1 </dev/null
rc=$?
[ "$rc" -gt 1 ] || pc_fail "a missing patch file gave status $rc, not above 1"
want5 t.txt a b c d e || pc_fail "the file was changed although the patch file is missing"
rm -f nosuchfile.rej
patch -i pchg nosuchfile >/dev/null 2>&1 </dev/null
rc=$?
[ "$rc" -ne 0 ] || pc_fail "a missing file to patch gave status 0"
patch -i pchg t.txt >/dev/null 2>&1 </dev/null
[ $? -eq 0 ] || pc_fail "a good patch did not give status 0"
pc_pass
