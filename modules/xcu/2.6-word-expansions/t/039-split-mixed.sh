# IFS white space next to a non-white-space IFS character is part of one
# delimiter, and leading and trailing IFS white space is ignored.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.2.6-word-expansions.039
. "$PC_CORE/pc.sh"
pc_begin XCU.2.6-word-expansions.039
PCSH=${PC_SH:-sh}
LC_ALL=C
export PCSH LC_ALL
PCW=$PWD
PCD=pcw$$
trap 'cd "$PCW" 2>/dev/null; rm -rf "$PCD"' 0
mkdir "$PCD" && cd "$PCD" || pc_fail "cannot make a working directory"
{
	printf 'IFS=\047 :\047\n'
	printf 'x=\047a : b\047; set -- $x; echo $# "[$1][$2]"\n'
	printf 'x=\047a :: b\047; set -- $x; echo $# "[$1][$2][$3]"\n'
	printf 'x=\047 a  b \047; set -- $x; echo $# "[$1][$2]"\n'
	printf 'x=\047 : a\047; set -- $x; echo $# "[$1][$2]"\n'
} >s.sh || pc_fail "cannot write the test script"
pc_test
"$PCSH" ./s.sh </dev/null >out 2>err
rc=$?
want=$(printf '2 [a][b]\n3 [a][][b]\n2 [a][b]\n2 [][a]\n'; printf x)
got=$(cat out; printf x)
[ "$got" = "$want" ] || pc_fail "standard output was [$(cat out)]"
pc_pass
