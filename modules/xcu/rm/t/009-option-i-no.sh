# With -i, rm prompts on standard error naming the file; a negative reply keeps the file and the status is 0.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.rm.009
. "$PC_CORE/pc.sh"
pc_begin XCU.rm.009
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
printf 'n\n' | rm -i victim >out 2>err
rc=$?
[ "$rc" -eq 0 ] || pc_fail "rm -i exited with status $rc after a negative reply"
[ -f victim ] || pc_fail "victim was removed"
slurp err
case $data in
*victim*) ;;
*) pc_fail "the prompt does not contain the file name" ;;
esac
pc_pass
