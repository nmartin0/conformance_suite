# ls -m starts a new line, after the comma, when the next name does not fit the line length given by COLUMNS.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.ls.005
. "$PC_CORE/pc.sh"
pc_begin XCU.ls.005
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
for n in aaaa bbbb cccc; do
	: >"d/$n" || pc_fail "cannot create d/$n"
done
pc_test
COLUMNS=8 ls -m d >out 2>err </dev/null
rc=$?
[ "$rc" -eq 0 ] || pc_fail "ls -m exited with status $rc"
sed 's/ *$//' out >out2 || pc_fail "cannot post-process the output"
slurp out2
[ "$data" = "aaaa,${nl}bbbb,${nl}cccc${nl}" ] || pc_fail "ls -m did not wrap as expected"
pc_pass
