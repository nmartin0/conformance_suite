# ls -S sorts by size, largest first, ties by name; with -r the smallest comes first.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.ls.025
# MUTANT    ls.S_ignored
# MUTANT    ls.S_ascending
# MUTANT    ls.S_r_ignored
# MUTANT    ls.S_ties_descending
. "$PC_CORE/pc.sh"
pc_begin XCU.ls.025
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
mkdir d e || pc_fail "cannot create directories"
printf '%01d' 0 >d/a || pc_fail "cannot create d/a"
printf '%0100d' 0 >d/b || pc_fail "cannot create d/b"
printf '%010d' 0 >d/c || pc_fail "cannot create d/c"
printf '%010d' 0 >d/c2 || pc_fail "cannot create d/c2"
printf '%010d' 0 >e/p || pc_fail "cannot create e/p"
printf '%0100d' 0 >e/q || pc_fail "cannot create e/q"
printf '%01d' 0 >e/r || pc_fail "cannot create e/r"
pc_test
ls -S d >out 2>err </dev/null
rc=$?
[ "$rc" -eq 0 ] || pc_fail "ls -S exited with status $rc"
slurp out
[ "$data" = "b${nl}c${nl}c2${nl}a${nl}" ] || pc_fail "ls -S output is wrong"
ls -rS e >out 2>err </dev/null
slurp out
[ "$data" = "r${nl}p${nl}q${nl}" ] || pc_fail "ls -rS output is wrong"
pc_pass
