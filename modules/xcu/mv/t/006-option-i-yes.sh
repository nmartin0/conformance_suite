# With -i, an affirmative reply makes mv replace the destination.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.mv.006
. "$PC_CORE/pc.sh"
pc_begin XCU.mv.006
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

printf 'new\n' >a || pc_fail "cannot create a"
printf 'old\n' >bfile || pc_fail "cannot create bfile"
pc_test
printf 'y\n' | mv -i a bfile >out 2>err
rc=$?
[ "$rc" -eq 0 ] || pc_fail "mv -i exited with status $rc after an affirmative reply"
[ ! -e a ] || pc_fail "source still exists"
slurp bfile
[ "$data" = "new${nl}" ] || pc_fail "destination was not replaced"
slurp err
case $data in
*bfile*) ;;
*) pc_fail "no prompt naming the destination" ;;
esac
pc_pass
