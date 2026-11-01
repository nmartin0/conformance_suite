# With -b and -o, file.orig is not created, and outfile.orig is created when outfile already exists.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.patch.015
# MUTANT    patch.bo_file_orig_created
# MUTANT    patch.bo_orig_is_new
# MUTANT    patch.bo_orig_not_made
# MUTANT    patch.bo_original_modified
. "$PC_CORE/pc.sh"
pc_begin XCU.patch.015
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
rm -f out out.orig t.txt.orig
printf 'old\n' >out || pc_unresolved "cannot create input"
patch -b -o out -i pchg t.txt >/dev/null 2>&1 </dev/null
want5 out a b X d e || pc_fail "-b -o with an existing outfile: the output file is not a b X d e"
[ -f out.orig ] || pc_fail "-b -o with an existing outfile did not create outfile.orig"
want5 out.orig old || pc_fail "outfile.orig does not hold the old outfile"
[ ! -e t.txt.orig ] || pc_fail "-b -o created file.orig"
want5 t.txt a b c d e || pc_fail "-b -o: the original file was modified"
pc_pass
