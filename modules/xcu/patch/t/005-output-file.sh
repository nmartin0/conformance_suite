# patch -o writes the patched file to the named file and leaves the original alone.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.patch.005
# MUTANT    patch.i_ignored
# MUTANT    patch.status1
# MUTANT    patch.last_newline_dropped
# MUTANT    patch.o_ignored
# MUTANT    patch.o_original_modified
# MUTANT    patch.o_to_stdout
# MUTANT    patch.o_out_is_original
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
pc_pass
