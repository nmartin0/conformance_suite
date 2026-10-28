# ls -S sorts by size, largest first, ties by name; with -r the smallest comes first.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.ls.025
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
printf '%0100d' 0 >d/big || pc_fail "cannot create d/big"
printf '%010d' 0 >d/mid2 || pc_fail "cannot create d/mid2"
printf '%010d' 0 >d/mid || pc_fail "cannot create d/mid"
printf '%01d' 0 >d/small || pc_fail "cannot create d/small"
printf '%0100d' 0 >e/big || pc_fail "cannot create e/big"
printf '%010d' 0 >e/mid || pc_fail "cannot create e/mid"
printf '%01d' 0 >e/small || pc_fail "cannot create e/small"
pc_test
ls -S d >out 2>err </dev/null
rc=$?
[ "$rc" -eq 0 ] || pc_fail "ls -S exited with status $rc"
slurp out
[ "$data" = "big${nl}mid${nl}mid2${nl}small${nl}" ] || pc_fail "ls -S output is wrong"
ls -rS e >out 2>err </dev/null
slurp out
[ "$data" = "small${nl}mid${nl}big${nl}" ] || pc_fail "ls -rS output is wrong"
pc_pass
