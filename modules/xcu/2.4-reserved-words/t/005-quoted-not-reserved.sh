# A word is a reserved word only when none of its characters is quoted: if,
# while and the like written with a backslash or quotes in them are ordinary
# command names (here not found, status 127).
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.2.4-reserved-words.005
. "$PC_CORE/pc.sh"
pc_begin XCU.2.4-reserved-words.005
PCSH=${PC_SH:-sh}
LC_ALL=C
export PCSH LC_ALL
PCW=$PWD
PCD=pcw$$
trap 'cd "$PCW" 2>/dev/null; rm -rf "$PCD"' 0
mkdir "$PCD" && cd "$PCD" || pc_fail "cannot make a working directory"
{
	printf '(\\if true) 2>/dev/null\n'
	printf 'echo $?\n'
	printf '(\047if\047 true) 2>/dev/null\n'
	printf 'echo $?\n'
	printf '(wh"ile" true) 2>/dev/null\n'
	printf 'echo $?\n'
} >s.sh || pc_fail "cannot write the test script"
pc_test
"$PCSH" ./s.sh </dev/null >out 2>err
rc=$?
want=$(printf '127\n127\n127\n'; printf x)
got=$(cat out; printf x)
[ "$got" = "$want" ] || pc_fail "standard output was [$(cat out)]"
pc_pass
