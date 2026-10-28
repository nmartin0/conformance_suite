# ls -R descends into a subdirectory whose name starts with a period only if that name was listed.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.ls.020
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
mkdir d d/.hid d/vis || pc_fail "cannot create directories"
: >d/.hid/inner || pc_fail "cannot create d/.hid/inner"
: >d/vis/f || pc_fail "cannot create d/vis/f"
pc_test
ls -R d >out 2>err </dev/null
slurp out
case $data in
"d:$nl"*) data=${data#"d:$nl"} ;;
esac
[ "$data" = "vis${nl}${nl}d/vis:${nl}f${nl}" ] || pc_fail "ls -R output is wrong"
ls -RA d >out 2>err </dev/null
slurp out
case $data in
"d:$nl"*) data=${data#"d:$nl"} ;;
esac
[ "$data" = ".hid${nl}vis${nl}${nl}d/.hid:${nl}inner${nl}${nl}d/vis:${nl}f${nl}" ] || pc_fail "ls -RA output is wrong"
pc_pass
