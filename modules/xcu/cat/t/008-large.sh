# cat copies a file of several thousand lines without loss.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.cat.008
. "$PC_CORE/pc.sh"
pc_begin XCU.cat.008
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

i=0
while [ "$i" -lt 3000 ]; do
	echo "line $i of the big file"
	i=$((i + 1))
done >big || pc_fail "cannot create big"
pc_test
cat big >out 2>err </dev/null
rc=$?
[ "$rc" -eq 0 ] || pc_fail "cat exited with status $rc"
slurp big
want=$data
slurp out
[ "$data" = "$want" ] || pc_fail "copy differs from the original"
pc_pass
