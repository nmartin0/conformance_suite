# ls -m writes the names across the page separated by a comma and a space, ending with a newline.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.ls.004
# MUTANT    ls.m_nospace
# MUTANT    ls.m_no_newline
# MUTANT    ls.m_trailing_comma
# MUTANT    ls.m_semicolon
# MUTANT    ls.m_reverse
# MUTANT    ls.m_ignored
# VARIANT   ls.dot_A
# VARIANT   ls.dot_a
. "$PC_CORE/pc.sh"
pc_begin XCU.ls.004
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
# File operands: a directory listing may also hold dot and dot-dot (DESCRIPTION).
COLUMNS=80 ls -m d/b d/a d/C >out 2>err </dev/null
rc=$?
[ "$rc" -eq 0 ] || pc_fail "ls -m exited with status $rc"
slurp out
[ "$data" = "d/C, d/a, d/b${nl}" ] || pc_fail "ls -m output is wrong"
pc_pass
