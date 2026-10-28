# The word of a ${ } modifier is expanded only when its value is needed:
# with the parameter set and not null no side effect of the word takes
# place.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.2.6-word-expansions.015
. "$PC_CORE/pc.sh"
pc_begin XCU.2.6-word-expansions.015
PCSH=${PC_SH:-sh}
LC_ALL=C
export PCSH LC_ALL
PCW=$PWD
PCD=pcw$$
trap 'cd "$PCW" 2>/dev/null; rm -rf "$PCD"' 0
mkdir "$PCD" && cd "$PCD" || pc_fail "cannot make a working directory"
{
	printf 'y=0; x=set; : ${x:-$((y=1))}; echo $y\n'
	printf 'unset x; : ${x:-$((y=1))}; echo $y\n'
} >s.sh || pc_fail "cannot write the test script"
pc_test
"$PCSH" ./s.sh </dev/null >out 2>err
rc=$?
want=$(printf '0\n1\n'; printf x)
got=$(cat out; printf x)
[ "$got" = "$want" ] || pc_fail "standard output was [$(cat out)]"
pc_pass
