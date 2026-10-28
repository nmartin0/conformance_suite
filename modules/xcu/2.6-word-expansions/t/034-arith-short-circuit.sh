# As in C, the right side of && and || and the unchosen branch of the
# conditional operator are not evaluated.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.2.6-word-expansions.034
. "$PC_CORE/pc.sh"
pc_begin XCU.2.6-word-expansions.034
PCSH=${PC_SH:-sh}
LC_ALL=C
export PCSH LC_ALL
PCW=$PWD
PCD=pcw$$
trap 'cd "$PCW" 2>/dev/null; rm -rf "$PCD"' 0
mkdir "$PCD" && cd "$PCD" || pc_fail "cannot make a working directory"
{
	printf 'x=0\n'
	printf ': $((0 && (x=1)))\n'
	printf 'echo $x\n'
	printf ': $((1 || (x=2)))\n'
	printf 'echo $x\n'
	printf ': $((1 ? (x=3) : (x=4)))\n'
	printf 'echo $x\n'
	printf ': $((0 ? (x=5) : (x=6)))\n'
	printf 'echo $x\n'
} >s.sh || pc_fail "cannot write the test script"
pc_test
"$PCSH" ./s.sh </dev/null >out 2>err
rc=$?
want=$(printf '0\n0\n3\n6\n'; printf x)
got=$(cat out; printf x)
[ "$got" = "$want" ] || pc_fail "standard output was [$(cat out)]"
pc_pass
