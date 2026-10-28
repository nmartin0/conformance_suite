# ls -F marks directories with /, executables with *, FIFOs with | and symbolic links with @.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.ls.007
. "$PC_CORE/pc.sh"
pc_begin XCU.ls.007
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
mkdir d d/dd || pc_fail "cannot create directories"
: >d/ex || pc_fail "cannot create d/ex"
chmod 755 d/ex || pc_fail "cannot chmod d/ex"
: >d/fl || pc_fail "cannot create d/fl"
ln -s fl d/lk || pc_fail "cannot create symbolic link"
mkfifo d/pp || pc_fail "cannot create FIFO"
pc_test
ls -F d >out 2>err </dev/null
rc=$?
[ "$rc" -eq 0 ] || pc_fail "ls -F exited with status $rc"
slurp out
[ "$data" = "dd/${nl}ex*${nl}fl${nl}lk@${nl}pp|${nl}" ] || pc_fail "ls -F output is wrong"
pc_pass
