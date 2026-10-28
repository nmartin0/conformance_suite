# patch applies a normal-format difference listing, whether named with -n or recognised, read from a file or the standard input.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.patch.001
. "$PC_CORE/pc.sh"
pc_begin XCU.patch.001
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
printf '2a3\n> Y\n' >padd || pc_unresolved "cannot create patch"
printf '2d1\n< b\n' >pdel || pc_unresolved "cannot create patch"
printf '2,3c2,4\n< b\n< c\n---\n> P\n> Q\n> R\n' >prng || pc_unresolved "cannot create patch"
pc_test
base
patch -n -i pchg t.txt >/dev/null 2>&1 </dev/null
rc=$?
[ "$rc" -eq 0 ] || pc_fail "-n change: status $rc, not 0"
want5 t.txt a b X d e || pc_fail "-n change: the file is not a b X d e"
base
patch -i pchg t.txt >/dev/null 2>&1 </dev/null
[ $? -eq 0 ] || pc_fail "auto-detected normal change: status not 0"
want5 t.txt a b X d e || pc_fail "auto-detected normal change: the file is not a b X d e"
base
patch t.txt <pchg >/dev/null 2>&1
[ $? -eq 0 ] || pc_fail "normal change from standard input: status not 0"
want5 t.txt a b X d e || pc_fail "normal change from standard input: the file is not a b X d e"
base
patch -i padd t.txt >/dev/null 2>&1 </dev/null
want5 t.txt a b Y c d e || pc_fail "normal add: the file is not a b Y c d e"
base
patch -i pdel t.txt >/dev/null 2>&1 </dev/null
want5 t.txt a c d e || pc_fail "normal delete: the file is not a c d e"
base
patch -i prng t.txt >/dev/null 2>&1 </dev/null
want5 t.txt a P Q R d e || pc_fail "normal range change: the file is not a P Q R d e"
pc_pass
