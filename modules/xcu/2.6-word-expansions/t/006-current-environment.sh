# Expansions of a command's name and arguments are carried out in the
# current shell environment, so an assignment made by ${v:=w} or by $((n=5))
# stays in effect, while one made inside a command substitution does not.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.2.6-word-expansions.006
. "$PC_CORE/pc.sh"
pc_begin XCU.2.6-word-expansions.006
PCSH=${PC_SH:-sh}
LC_ALL=C
export PCSH LC_ALL
PCW=$PWD
PCD=pcw$$
trap 'cd "$PCW" 2>/dev/null; rm -rf "$PCD"' 0
mkdir "$PCD" && cd "$PCD" || pc_fail "cannot make a working directory"
{
	printf 'unset v n\n'
	printf ': ${v:=set}\n'
	printf 'echo $v\n'
	printf ': $((n=5))\n'
	printf 'echo $n\n'
	printf ': $(m=3)\n'
	printf 'echo ${m-unset}\n'
} >s.sh || pc_fail "cannot write the test script"
pc_test
"$PCSH" ./s.sh </dev/null >out 2>err
rc=$?
want=$(printf 'set\n5\nunset\n'; printf x)
got=$(cat out; printf x)
[ "$got" = "$want" ] || pc_fail "standard output was [$(cat out)]"
pc_pass
