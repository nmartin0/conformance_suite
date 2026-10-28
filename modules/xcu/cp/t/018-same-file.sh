# When source and destination are the same file, cp does nothing more with it: the contents survive.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.cp.018
. "$PC_CORE/pc.sh"
pc_begin XCU.cp.018
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

printf 'keep\n' >f || pc_fail "cannot create f"
ln f g || pc_fail "cannot create hard link"
pc_test
cp f f >/dev/null 2>&1 </dev/null
cp f g >/dev/null 2>&1 </dev/null
slurp f
[ "$data" = "keep${nl}" ] || pc_fail "f lost its contents"
slurp g
[ "$data" = "keep${nl}" ] || pc_fail "g lost its contents"
pc_pass
