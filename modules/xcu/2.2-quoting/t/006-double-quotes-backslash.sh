# Inside double quotes a backslash is an escape only before a dollar sign,
# backquote, backslash, double quote or newline; before any other character
# both the backslash and that character stay.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.2.2-quoting.006
. "$PC_CORE/pc.sh"
pc_begin XCU.2.2-quoting.006
PCSH=${PC_SH:-sh}
LC_ALL=C
export PCSH LC_ALL
PCW=$PWD
PCD=pcw$$
trap 'cd "$PCW" 2>/dev/null; rm -rf "$PCD"' 0
mkdir "$PCD" && cd "$PCD" || pc_fail "cannot make a working directory"
{
	printf 'printf \047%%s\\n\047 "\\$x \\`x\\` \\\\ \\" \\a \\n \\q \\\047 \\!"\n'
	printf 'printf \047%%s\\n\047 "p\\\n'
	printf 'q"\n'
} >s.sh || pc_fail "cannot write the test script"
pc_test
"$PCSH" ./s.sh </dev/null >out 2>err
rc=$?
want=$(printf '$x `x` \\ " \\a \\n \\q \\\047 \\!\npq\n'; printf x)
got=$(cat out; printf x)
[ "$got" = "$want" ] || pc_fail "standard output was [$(cat out)]"
pc_pass
