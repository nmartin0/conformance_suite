# The shell sets IFS to space, tab and newline when it starts, whatever IFS
# was in the environment.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.2.5-parameters.013
. "$PC_CORE/pc.sh"
pc_begin XCU.2.5-parameters.013
PCSH=${PC_SH:-sh}
LC_ALL=C
export PCSH LC_ALL
PCW=$PWD
PCD=pcw$$
trap 'cd "$PCW" 2>/dev/null; rm -rf "$PCD"' 0
mkdir "$PCD" && cd "$PCD" || pc_fail "cannot make a working directory"
{
	printf 'x=$(printf \047 \\t\\nX\047)\n'
	printf 'if [ "$IFS" = "${x%%X}" ]; then echo ok; else echo bad; fi\n'
} >s.sh || pc_fail "cannot write the test script"
pc_test
IFS=: "$PCSH" ./s.sh </dev/null >out 2>err
rc=$?
want=$(printf 'ok\n'; printf x)
got=$(cat out; printf x)
[ "$got" = "$want" ] || pc_fail "standard output was [$(cat out)]"
pc_pass
