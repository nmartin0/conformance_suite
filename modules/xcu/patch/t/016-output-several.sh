# With -o, several patches for one file are applied one after another to the intermediate versions, and the versions are written to outfile one after the other.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.patch.016
# MUTANT    patch.o_several_last_only
# MUTANT    patch.o_several_first_only
# MUTANT    patch.o_several_from_original
# MUTANT    patch.o_several_overwrites
. "$PC_CORE/pc.sh"
pc_begin XCU.patch.016
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
printf 'Index: t.txt\n3c3\n< c\n---\n> X\nIndex: t.txt\n4c4\n< d\n---\n> Y\n' >ptwo || pc_unresolved "cannot create patch"
pc_test
base
rm -f out
patch -o out -i ptwo t.txt >/dev/null 2>&1 </dev/null
want5 out a b X d e a b X Y e || pc_fail "-o with two patches for one file: the output file is not the two versions one after the other (a b X d e, then a b X Y e)"
want5 t.txt a b c d e || pc_fail "-o with two patches: the original file was modified"
pc_pass
