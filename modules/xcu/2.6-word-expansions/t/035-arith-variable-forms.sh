# When a variable holds an integer constant, optionally with a leading plus
# or minus, $((x)) and $(($x)) give the same value.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.2.6-word-expansions.035
. "$PC_CORE/pc.sh"
pc_begin XCU.2.6-word-expansions.035
PCSH=${PC_SH:-sh}
LC_ALL=C
export PCSH LC_ALL
PCW=$PWD
PCD=pcw$$
trap 'cd "$PCW" 2>/dev/null; rm -rf "$PCD"' 0
mkdir "$PCD" && cd "$PCD" || pc_fail "cannot make a working directory"
{
	printf 'for v in 5 -5 +5 010 0x1f -010 +0x10; do\n'
	printf 'x=$v\n'
	printf 'a=$((x))\n'
	printf 'b=$(($x))\n'
	printf 'if [ "$a" = "$b" ]; then echo "$v ok $a"; else echo "$v bad $a $b"; fi\n'
	printf 'done\n'
} >s.sh || pc_fail "cannot write the test script"
pc_test
"$PCSH" ./s.sh </dev/null >out 2>err
rc=$?
want=$(printf '5 ok 5\n-5 ok -5\n+5 ok 5\n010 ok 8\n0x1f ok 31\n-010 ok -8\n+0x10 ok 16\n'; printf x)
got=$(cat out; printf x)
[ "$got" = "$want" ] || pc_fail "standard output was [$(cat out)]"
pc_pass
