# ls -R does not descend through a symbolic link to a directory unless -L is given.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.ls.021
# MUTANT    ls.R_follows_links
# MUTANT    ls.R_L_ignored
# VARIANT   ls.dot_A
# VARIANT   ls.dot_a
# VARIANT   ls.R_omit_first_header
. "$PC_CORE/pc.sh"
pc_begin XCU.ls.021
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
mkdir d other || pc_fail "cannot create directories"
: >other/zzz || pc_fail "cannot create other/zzz"
ln -s ../other d/lk || pc_fail "cannot create symbolic link"
pc_test
ls -R d >out 2>err </dev/null
nodot out
case $data in
"d:$nl"*) data=${data#"d:$nl"} ;;
esac
[ "$data" = "lk${nl}" ] || pc_fail "ls -R followed the link or listed wrongly"
ls -RL d >out 2>err </dev/null
nodot out
case $data in
"d:$nl"*) data=${data#"d:$nl"} ;;
esac
[ "$data" = "lk${nl}${nl}d/lk:${nl}zzz${nl}" ] || pc_fail "ls -RL did not descend through the link"
pc_pass
