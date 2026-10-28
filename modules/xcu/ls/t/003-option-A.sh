# ls -A lists names that start with a period but never dot or dot-dot.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.ls.003
. "$PC_CORE/pc.sh"
pc_begin XCU.ls.003
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
mkdir d || pc_fail "cannot create d"
for n in b a C .h; do
	: >"d/$n" || pc_fail "cannot create d/$n"
done
pc_test
ls -A d >out 2>err </dev/null
rc=$?
[ "$rc" -eq 0 ] || pc_fail "ls -A exited with status $rc"
slurp out
[ "$data" = ".h${nl}C${nl}a${nl}b${nl}" ] || pc_fail "ls -A lists the wrong entries"
pc_pass
