# The result of a parameter expansion is not expanded again (no further
# tilde, parameter, command or arithmetic expansion); only field splitting
# and filename expansion follow it.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.2.6-word-expansions.001
. "$PC_CORE/pc.sh"
pc_begin XCU.2.6-word-expansions.001
PCSH=${PC_SH:-sh}
LC_ALL=C
export PCSH LC_ALL
PCW=$PWD
PCD=pcw$$
trap 'cd "$PCW" 2>/dev/null; rm -rf "$PCD"' 0
mkdir "$PCD" && cd "$PCD" || pc_fail "cannot make a working directory"
{
	printf 'y=1\n'
	printf 'x=\047$y ~ $(echo no) $((2+2)) `echo no`\047\n'
	printf 'echo "$x"\n'
	printf 'set -- $x\n'
	printf 'echo $#\n'
} >s.sh || pc_fail "cannot write the test script"
pc_test
"$PCSH" ./s.sh </dev/null >out 2>err
rc=$?
want=$(printf '$y ~ $(echo no) $((2+2)) `echo no`\n7\n'; printf x)
got=$(cat out; printf x)
[ "$got" = "$want" ] || pc_fail "standard output was [$(cat out)]"
pc_pass
