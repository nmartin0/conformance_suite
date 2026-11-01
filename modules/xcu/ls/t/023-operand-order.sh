# With several operands ls writes the non-directories first, then each directory (sorted) under a blank line and a header with its pathname.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.ls.023
# MUTANT    ls.op_dirs_first
# MUTANT    ls.op_dirs_unsorted
# MUTANT    ls.op_files_unsorted
# MUTANT    ls.op_no_blank
# MUTANT    ls.op_no_header
# MUTANT    ls.op_leading_blank
. "$PC_CORE/pc.sh"
pc_begin XCU.ls.023
nl='
'
slurp() {
	data=
	l=
	while IFS= read -r l; do
		data=$data$l$nl
		l=
	done <"$1"
	[ -z "$l" ] || data=$data$l'<noeol>'
	return 0
}

LC_ALL=POSIX; export LC_ALL
mkdir d1 d2 || pc_fail "cannot create directories"
: >f1 || pc_fail "cannot create f1"
: >f2 || pc_fail "cannot create f2"
: >d1/x || pc_fail "cannot create d1/x"
: >d2/y || pc_fail "cannot create d2/y"
pc_test
ls f2 d2 f1 d1 >out 2>err </dev/null
rc=$?
[ "$rc" -eq 0 ] || pc_fail "ls exited with status $rc"
slurp out
[ "$data" = "f1${nl}f2${nl}${nl}d1:${nl}x${nl}${nl}d2:${nl}y${nl}" ] || pc_fail "output is wrong"
ls d2 d1 >out 2>err </dev/null
slurp out
[ "$data" = "d1:${nl}x${nl}${nl}d2:${nl}y${nl}" ] || pc_fail "output for two directories is wrong"
pc_pass
