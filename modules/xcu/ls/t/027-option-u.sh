# With -t, -u sorts by the time of last access instead of modification.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.ls.027
# MUTANT    ls.u_ignored
# MUTANT    ls.u_is_c
# MUTANT    ls.u_l_mtime
# VARIANT   ls.dot_A
# VARIANT   ls.dot_a
. "$PC_CORE/pc.sh"
pc_begin XCU.ls.027
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
# A listing may also hold dot and dot-dot (DESCRIPTION): they are dropped.
nodot() {
	grep -v -F -x -e . -e .. "$1" >"$1.nd"
	slurp "$1.nd"
}
mkdir d || pc_fail "cannot create d"
touch -m -t 200101010101 d/p || pc_fail "cannot set time"
touch -m -t 200201010101 d/q || pc_fail "cannot set time"
touch -m -t 200301010101 d/r || pc_fail "cannot set time"
# The status change times follow the order of these three commands, which
# is neither the order of the modification times nor of the access times.
touch -a -t 200501010101 d/p || pc_fail "cannot set time"
sleep 2
touch -a -t 200401010101 d/q || pc_fail "cannot set time"
sleep 2
touch -a -t 200301010101 d/r || pc_fail "cannot set time"
pc_test
ls -t d >out 2>err </dev/null
nodot out
[ "$data" = "r${nl}q${nl}p${nl}" ] || pc_fail "ls -t (modification time) output is wrong"
ls -tu d >out 2>err </dev/null
rc=$?
[ "$rc" -eq 0 ] || pc_fail "ls -tu exited with status $rc"
nodot out
[ "$data" = "p${nl}q${nl}r${nl}" ] || pc_fail "ls -tu output is wrong"
l=$(ls -lu d/p) || pc_fail "ls -lu failed"
set -- $l
[ "$#" -eq 9 ] || pc_unresolved "unexpected number of fields ($#)"
[ "$6 $7 $8" = "Jan 1 2005" ] || pc_fail "ls -lu writes '$6 $7 $8' for a file last accessed on 1 Jan 2005"
l=$(ls -l d/p) || pc_fail "ls -l failed"
set -- $l
[ "$6 $7 $8" = "Jan 1 2001" ] || pc_fail "ls -l writes '$6 $7 $8' for a file modified on 1 Jan 2001"
pc_pass
