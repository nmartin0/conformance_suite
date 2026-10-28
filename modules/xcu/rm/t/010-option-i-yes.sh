# With -i, an affirmative reply makes rm remove the file.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.rm.010
. "$PC_CORE/pc.sh"
pc_begin XCU.rm.010
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

printf x >victim || pc_fail "cannot create victim"
pc_test
printf 'y\n' | rm -i victim >out 2>err
rc=$?
[ "$rc" -eq 0 ] || pc_fail "rm -i exited with status $rc after an affirmative reply"
[ ! -e victim ] || pc_fail "victim was not removed"
slurp err
case $data in
*victim*) ;;
*) pc_fail "no prompt naming the file" ;;
esac
pc_pass
