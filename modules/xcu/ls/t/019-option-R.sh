# ls -R lists a directory and then each subdirectory below it, each preceded by a blank line and a header with its pathname.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.ls.019
. "$PC_CORE/pc.sh"
pc_begin XCU.ls.019
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
: >d/a || pc_fail "cannot create d/a"
: >d/sub/f || pc_fail "cannot create d/sub/f"
pc_test
ls -R d >out 2>err </dev/null
rc=$?
[ "$rc" -eq 0 ] || pc_fail "ls -R exited with status $rc"
slurp out
case $data in
"d:$nl"*) data=${data#"d:$nl"} ;;
esac
[ "$data" = "a${nl}sub${nl}${nl}d/sub:${nl}f${nl}" ] || pc_fail "ls -R output is wrong"
pc_pass
