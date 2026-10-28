# With the default IFS a run of spaces, tabs and newlines in an unquoted
# expansion result separates fields, and leading or trailing white space
# gives no empty field.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.2.6-word-expansions.037
. "$PC_CORE/pc.sh"
pc_begin XCU.2.6-word-expansions.037
PCSH=${PC_SH:-sh}
LC_ALL=C
export PCSH LC_ALL
PCW=$PWD
PCD=pcw$$
trap 'cd "$PCW" 2>/dev/null; rm -rf "$PCD"' 0
mkdir "$PCD" && cd "$PCD" || pc_fail "cannot make a working directory"
{
	printf 'x=$(printf \047  a   b\\tc\\nd  \047)\n'
	printf 'set -- $x\n'
	printf 'echo $#\n'
	printf 'printf \047[%%s]\047 "$@"\n'
	printf 'echo\n'
} >s.sh || pc_fail "cannot write the test script"
pc_test
"$PCSH" ./s.sh </dev/null >out 2>err
rc=$?
want=$(printf '4\n[a][b][c][d]\n'; printf x)
got=$(cat out; printf x)
[ "$got" = "$want" ] || pc_fail "standard output was [$(cat out)]"
pc_pass
