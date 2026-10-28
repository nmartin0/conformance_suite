# rm -v writes a message containing the pathname of each file it removed to standard output.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.rm.013
. "$PC_CORE/pc.sh"
pc_begin XCU.rm.013
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

printf x >vfile1 || pc_fail "cannot create vfile1"
printf x >vfile2 || pc_fail "cannot create vfile2"
pc_test
rm -v vfile1 vfile2 >out 2>err </dev/null
rc=$?
[ "$rc" -eq 0 ] || pc_fail "rm -v exited with status $rc"
[ ! -e vfile1 ] && [ ! -e vfile2 ] || pc_fail "files were not removed"
slurp out
case $data in
*vfile1*) ;;
*) pc_fail "no message naming vfile1" ;;
esac
case $data in
*vfile2*) ;;
*) pc_fail "no message naming vfile2" ;;
esac
pc_pass
