# ls -F marks directories with /, executables with *, FIFOs with | and symbolic links with @.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.ls.007
# MUTANT    ls.F_ignored
# MUTANT    ls.F_no_exec
# MUTANT    ls.F_no_fifo
# MUTANT    ls.F_no_link
# MUTANT    ls.F_no_dir
# MUTANT    ls.F_star_all
# MUTANT    ls.F_fifo_equals
# VARIANT   ls.dot_A
# VARIANT   ls.dot_a
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

# A listing may also hold dot and dot-dot (DESCRIPTION), with their indicator.
nodot() {
	grep -v -F -x -e . -e .. -e ./ -e ../ "$1" >"$1.nd"
	slurp "$1.nd"
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
nodot out
[ "$data" = "dd/${nl}ex*${nl}fl${nl}lk@${nl}pp|${nl}" ] || pc_fail "ls -F output is wrong"
pc_pass
