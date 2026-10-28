# ${#p} is the decimal length in characters of the value of p, 0 for a null
# value; ${#} is the number of positional parameters.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.2.6-word-expansions.017
. "$PC_CORE/pc.sh"
pc_begin XCU.2.6-word-expansions.017
PCSH=${PC_SH:-sh}
LC_ALL=C
export PCSH LC_ALL
PCW=$PWD
PCD=pcw$$
trap 'cd "$PCW" 2>/dev/null; rm -rf "$PCD"' 0
mkdir "$PCD" && cd "$PCD" || pc_fail "cannot make a working directory"
{
	printf 'x=hello; echo ${#x}\n'
	printf 'x=; echo ${#x}\n'
	printf 'set -- abc d\n'
	printf 'echo ${#1} ${#2} ${#}\n'
	printf 'y=a; echo ${#y}${#y}\n'
} >s.sh || pc_fail "cannot write the test script"
pc_test
"$PCSH" ./s.sh </dev/null >out 2>err
rc=$?
want=$(printf '5\n0\n3 1 2\n11\n'; printf x)
got=$(cat out; printf x)
[ "$got" = "$want" ] || pc_fail "standard output was [$(cat out)]"
pc_pass
