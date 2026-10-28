# cmp -l writes, for each differing byte, its number in decimal and the two bytes in octal, and exits 1.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.cmp.003
. "$PC_CORE/pc.sh"
pc_begin XCU.cmp.003
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

printf 'abcdef' >f1 || pc_fail "cannot create f1"
printf 'aXcYef' >f2 || pc_fail "cannot create f2"
pc_test
cmp -l f1 f2 >out 2>err </dev/null
rc=$?
[ "$rc" -eq 1 ] || pc_fail "cmp -l exited with status $rc, expected 1"
slurp out
[ "$data" = "2 142 130${nl}4 144 131${nl}" ] || pc_fail "output is: $data"
pc_pass
