# ls -R descends into a subdirectory whose name starts with a period only if that name was listed.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.ls.020
# MUTANT    ls.R_descends_hidden
# MUTANT    ls.R_A_hidden_not_entered
# VARIANT   ls.dot_A
# VARIANT   ls.dot_a
# VARIANT   ls.R_omit_first_header
. "$PC_CORE/pc.sh"
pc_begin XCU.ls.020
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
mkdir d d/.hid d/vis || pc_fail "cannot create directories"
: >d/.hid/inner || pc_fail "cannot create d/.hid/inner"
: >d/vis/f || pc_fail "cannot create d/vis/f"
pc_test
ls -R d >out 2>err </dev/null
nodot out
case $data in
"d:$nl"*) data=${data#"d:$nl"} ;;
esac
# a name that begins with a period may be written without -A or -a when an
# implementation-defined condition holds (DESCRIPTION); .hid is then listed
# and so is recursed into, exactly as with -A
if [ "$data" != "vis${nl}${nl}d/vis:${nl}f${nl}" ]; then
	[ "$data" = ".hid${nl}vis${nl}${nl}d/.hid:${nl}inner${nl}${nl}d/vis:${nl}f${nl}" ] || pc_fail "ls -R output is wrong"
fi
ls -RA d >out 2>err </dev/null
nodot out
case $data in
"d:$nl"*) data=${data#"d:$nl"} ;;
esac
[ "$data" = ".hid${nl}vis${nl}${nl}d/.hid:${nl}inner${nl}${nl}d/vis:${nl}f${nl}" ] || pc_fail "ls -RA output is wrong"
pc_pass
