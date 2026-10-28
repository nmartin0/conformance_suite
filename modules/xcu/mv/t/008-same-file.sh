# When source and destination are the same file, the destination is not removed and the contents survive (the source name may go when the two names are distinct entries).
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.mv.008
. "$PC_CORE/pc.sh"
pc_begin XCU.mv.008
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

printf 'k\n' >a || pc_fail "cannot create a"
ln a b || pc_fail "cannot create hard link"
pc_test
mv a b >/dev/null 2>&1 </dev/null
[ -f b ] || pc_fail "the destination b was removed"
slurp b
[ "$data" = "k${nl}" ] || pc_fail "b lost its contents"
[ -f a ] || ln b a || pc_unresolved "cannot restore the second name"
mv a ./a >/dev/null 2>&1 </dev/null
slurp a
[ "$data" = "k${nl}" ] || pc_fail "a lost its contents"
pc_pass
