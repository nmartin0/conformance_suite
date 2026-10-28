# When IFS is unset the shell splits fields as if it held space, tab and
# newline, and the variable stays unset.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.2.5-parameters.014
. "$PC_CORE/pc.sh"
pc_begin XCU.2.5-parameters.014
PCSH=${PC_SH:-sh}
LC_ALL=C
export PCSH LC_ALL
PCW=$PWD
PCD=pcw$$
trap 'cd "$PCW" 2>/dev/null; rm -rf "$PCD"' 0
mkdir "$PCD" && cd "$PCD" || pc_fail "cannot make a working directory"
{
	printf 'unset IFS\n'
	printf 'x=$(printf \047a b\\tc\\nd\047)\n'
	printf 'set -- $x\n'
	printf 'echo $#\n'
	printf 'if [ -z "${IFS+x}" ]; then echo unset; else echo set; fi\n'
} >s.sh || pc_fail "cannot write the test script"
pc_test
"$PCSH" ./s.sh </dev/null >out 2>err
rc=$?
want=$(printf '4\nunset\n'; printf x)
got=$(cat out; printf x)
[ "$got" = "$want" ] || pc_fail "standard output was [$(cat out)]"
pc_pass
