# When no part of the delimiter word is quoted, the lines of a here-document
# undergo parameter expansion, command substitution and arithmetic
# expansion, a backslash acts as in double quotes (so it escapes a dollar
# sign, backquote, backslash or newline and is otherwise literal), and
# quotation marks are ordinary characters.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.2.7-redirection.015
. "$PC_CORE/pc.sh"
pc_begin XCU.2.7-redirection.015
PCSH=${PC_SH:-sh}
LC_ALL=C
export PCSH LC_ALL
PCW=$PWD
PCD=pcw$$
trap 'cd "$PCW" 2>/dev/null; rm -rf "$PCD"' 0
mkdir "$PCD" && cd "$PCD" || pc_fail "cannot make a working directory"
{
	printf 'x=v\n'
	printf 'cat <<E\n'
	printf '$x ${x}w $(echo c) `echo d` $((1+2)) \\$x \\\\ \\a "q" \047r\047 a\\\n'
	printf 'b\n'
	printf 'E\n'
} >s.sh || pc_fail "cannot write the test script"
pc_test
"$PCSH" ./s.sh </dev/null >out 2>err
rc=$?
want=$(printf 'v vw c d 3 $x \\ \\a "q" \047r\047 ab\n'; printf x)
got=$(cat out; printf x)
[ "$got" = "$want" ] || pc_fail "standard output was [$(cat out)]"
pc_pass
