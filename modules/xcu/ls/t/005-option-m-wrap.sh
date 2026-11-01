# ls -m starts a new line, after the comma, when the next name does not fit the line length given by COLUMNS.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.ls.005
# MUTANT    ls.m_nowrap
# MUTANT    ls.m_wrap_early
# MUTANT    ls.m_wrap_late
# MUTANT    ls.m_wrap_before_comma
# MUTANT    ls.m_wrap_no_comma
# VARIANT   ls.m_trailing_space
# VARIANT   ls.dot_A
# VARIANT   ls.dot_a
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
# File operands: a directory listing may also hold dot and dot-dot (DESCRIPTION).
cd d || pc_unresolved "cannot enter d"
COLUMNS=8 ls -m aaaa bbbb cccc >out 2>err </dev/null
rc=$?
[ "$rc" -eq 0 ] || pc_fail "ls -m exited with status $rc"
sed 's/ *$//' out >out2 || pc_fail "cannot post-process the output"
slurp out2
[ "$data" = "aaaa,${nl}bbbb,${nl}cccc${nl}" ] || pc_fail "ls -m did not wrap as expected"
# Room for a second name on the line: "aaaa, bbbb," is 11 columns, and a third
# name would make 16, more than 14.
COLUMNS=14 ls -m aaaa bbbb cccc >out 2>err </dev/null
rc=$?
[ "$rc" -eq 0 ] || pc_fail "ls -m exited with status $rc"
sed 's/ *$//' out >out2 || pc_fail "cannot post-process the output"
slurp out2
[ "$data" = "aaaa, bbbb,${nl}cccc${nl}" ] || pc_fail "ls -m did not fill the line before wrapping"
pc_pass
