# ls -r reverses the collating order.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.ls.024
. "$PC_CORE/pc.sh"
pc_begin XCU.ls.024
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
for n in b a C; do
	: >"d/$n" || pc_fail "cannot create d/$n"
done
pc_test
ls -r d >out 2>err </dev/null
rc=$?
[ "$rc" -eq 0 ] || pc_fail "ls -r exited with status $rc"
slurp out
[ "$data" = "b${nl}a${nl}C${nl}" ] || pc_fail "ls -r output is wrong"
pc_pass
