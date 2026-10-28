# With -d, -F or -l, a symbolic link to a directory given as operand is not followed.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.ls.031
. "$PC_CORE/pc.sh"
pc_begin XCU.ls.031
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
mkdir d d/sub || pc_fail "cannot create directories"
: >d/sub/x || pc_fail "cannot create d/sub/x"
ln -s sub d/sl || pc_fail "cannot create symbolic link"
pc_test
ls -d d/sl >out 2>err </dev/null
slurp out
[ "$data" = "d/sl${nl}" ] || pc_fail "ls -d: output is wrong"
ls -F d/sl >out 2>err </dev/null
slurp out
[ "$data" = "d/sl@${nl}" ] || pc_fail "ls -F: output is wrong"
ls -l d/sl >out 2>err </dev/null
n=$(sed -n '$=' out)
[ "$n" = 1 ] || pc_fail "ls -l wrote $n lines, expected 1"
l=$(cat out)
case $l in
l*" d/sl -> sub") ;;
*) pc_fail "ls -l: $l" ;;
esac
pc_pass
