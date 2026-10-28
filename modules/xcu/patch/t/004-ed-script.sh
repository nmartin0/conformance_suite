# patch -e applies a difference listing in the style of an ed script.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.patch.004
. "$PC_CORE/pc.sh"
pc_begin XCU.patch.004
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
printf '3c\nX\n.\n' >ped1 || pc_unresolved "cannot create patch"
printf '2a\nY\n.\n' >ped2 || pc_unresolved "cannot create patch"
printf '4d\n2c\nZ\n.\n' >ped3 || pc_unresolved "cannot create patch"
pc_test
base
patch -e -i ped1 t.txt >/dev/null 2>&1 </dev/null
[ $? -eq 0 ] || pc_fail "-e change: status not 0"
want5 t.txt a b X d e || pc_fail "-e change: the file is not a b X d e"
base
patch -e -i ped2 t.txt >/dev/null 2>&1 </dev/null
want5 t.txt a b Y c d e || pc_fail "-e add: the file is not a b Y c d e"
base
patch -e -i ped3 t.txt >/dev/null 2>&1 </dev/null
want5 t.txt a Z c e || pc_fail "-e delete and change: the file is not a Z c e"
base
patch -e t.txt <ped1 >/dev/null 2>&1
want5 t.txt a b X d e || pc_fail "-e from standard input: the file is not a b X d e"
pc_pass
