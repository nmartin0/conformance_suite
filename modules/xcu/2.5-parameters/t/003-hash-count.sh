# The special parameter # expands to the number of positional parameters,
# written in decimal, not counting parameter 0.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.2.5-parameters.003
. "$PC_CORE/pc.sh"
pc_begin XCU.2.5-parameters.003
PCSH=${PC_SH:-sh}
LC_ALL=C
export PCSH LC_ALL
PCW=$PWD
PCD=pcw$$
trap 'cd "$PCW" 2>/dev/null; rm -rf "$PCD"' 0
mkdir "$PCD" && cd "$PCD" || pc_fail "cannot make a working directory"
{
	printf 'set --\n'
	printf 'echo $#\n'
	printf 'set -- a b\n'
	printf 'echo $#\n'
	printf 'set -- a b c d e f g h i j k\n'
	printf 'echo $# "${#}"\n'
} >s.sh || pc_fail "cannot write the test script"
pc_test
"$PCSH" ./s.sh </dev/null >out 2>err
rc=$?
want=$(printf '0\n2\n11 11\n'; printf x)
got=$(cat out; printf x)
[ "$got" = "$want" ] || pc_fail "standard output was [$(cat out)]"
pc_pass
