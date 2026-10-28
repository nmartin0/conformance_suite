# patch -R applies a patch in the reverse sense, undoing it.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.patch.006
. "$PC_CORE/pc.sh"
pc_begin XCU.patch.006
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
tab=$(printf '\t')
{
	printf -- '--- t.txt%s2020-01-02 03:04:05.000000000 +0000\n' "$tab"
	printf '+++ t.txt%s2021-03-04 05:06:07.000000000 +0000\n' "$tab"
	printf '@@ -1,5 +1,5 @@\n a\n b\n-c\n+X\n d\n e\n'
} >puni || pc_unresolved "cannot create patch"
pc_test
base
patch -i pchg t.txt >/dev/null 2>&1 </dev/null
patch -R -i pchg t.txt >/dev/null 2>&1 </dev/null
rc=$?
[ "$rc" -eq 0 ] || pc_fail "-R of a normal patch: status $rc, not 0"
want5 t.txt a b c d e || pc_fail "-R of a normal patch did not restore a b c d e"
base
patch -u -i puni t.txt >/dev/null 2>&1 </dev/null
want5 t.txt a b X d e || pc_unresolved "the forward unified patch did not apply"
patch -R -u -i puni t.txt >/dev/null 2>&1 </dev/null
rc=$?
[ "$rc" -eq 0 ] || pc_fail "-R of a unified patch: status $rc, not 0"
want5 t.txt a b c d e || pc_fail "-R of a unified patch did not restore a b c d e"
pc_pass
