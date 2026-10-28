# Each non-white-space IFS character ends a field, so adjacent ones give
# empty fields, a leading one gives an empty first field, and a single
# trailing one adds no field; a lone one gives one empty field.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.2.6-word-expansions.038
. "$PC_CORE/pc.sh"
pc_begin XCU.2.6-word-expansions.038
PCSH=${PC_SH:-sh}
LC_ALL=C
export PCSH LC_ALL
PCW=$PWD
PCD=pcw$$
trap 'cd "$PCW" 2>/dev/null; rm -rf "$PCD"' 0
mkdir "$PCD" && cd "$PCD" || pc_fail "cannot make a working directory"
{
	printf 'IFS=:\n'
	printf 'x=a::b\n'
	printf 'set -- $x; echo $# "[$1][$2][$3]"\n'
	printf 'x=:a:\n'
	printf 'set -- $x; echo $# "[$1][$2]"\n'
	printf 'x=:\n'
	printf 'set -- $x; echo $# "[$1]"\n'
	printf 'x=a:\n'
	printf 'set -- $x; echo $# "[$1]"\n'
	printf 'x=::\n'
	printf 'set -- $x; echo $#\n'
} >s.sh || pc_fail "cannot write the test script"
pc_test
"$PCSH" ./s.sh </dev/null >out 2>err
rc=$?
want=$(printf '3 [a][][b]\n2 [][a]\n1 []\n1 [a]\n2\n'; printf x)
got=$(cat out; printf x)
[ "$got" = "$want" ] || pc_fail "standard output was [$(cat out)]"
pc_pass
