# The words for, in, do, done, while and until are reserved words that make
# the loop commands when they appear in command position.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.2.4-reserved-words.002
. "$PC_CORE/pc.sh"
pc_begin XCU.2.4-reserved-words.002
PCSH=${PC_SH:-sh}
LC_ALL=C
export PCSH LC_ALL
PCW=$PWD
PCD=pcw$$
trap 'cd "$PCW" 2>/dev/null; rm -rf "$PCD"' 0
mkdir "$PCD" && cd "$PCD" || pc_fail "cannot make a working directory"
{
	printf 'for i in 1 2 3; do echo $i; done\n'
	printf 'n=0\n'
	printf 'while [ $n -lt 2 ]; do echo w$n; n=$((n+1)); done\n'
	printf 'until [ $n -ge 4 ]; do echo u$n; n=$((n+1)); done\n'
} >s.sh || pc_fail "cannot write the test script"
pc_test
"$PCSH" ./s.sh </dev/null >out 2>err
rc=$?
want=$(printf '1\n2\n3\nw0\nw1\nu2\nu3\n'; printf x)
got=$(cat out; printf x)
[ "$got" = "$want" ] || pc_fail "standard output was [$(cat out)]"
pc_pass
