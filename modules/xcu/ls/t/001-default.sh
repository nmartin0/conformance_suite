# ls lists the visible entries of a directory one per line, sorted by the collating sequence (byte order in the POSIX locale), omitting names that start with a period; -1 and a later -1 after -C give the same.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.ls.001
. "$PC_CORE/pc.sh"
pc_begin XCU.ls.001
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
ls d >out 2>err </dev/null
rc=$?
[ "$rc" -eq 0 ] || pc_fail "ls exited with status $rc"
slurp out
[ "$data" = "C${nl}a${nl}b${nl}" ] || pc_fail "default listing is wrong"
ls -1 d >out 2>err </dev/null
slurp out
[ "$data" = "C${nl}a${nl}b${nl}" ] || pc_fail "ls -1 listing is wrong"
ls -C -1 d >out 2>err </dev/null
slurp out
[ "$data" = "C${nl}a${nl}b${nl}" ] || pc_fail "ls -C -1 listing is wrong"
pc_pass
