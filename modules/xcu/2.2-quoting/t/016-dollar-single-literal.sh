# Other characters inside dollar-single-quotes keep their literal value, so
# dollar signs, backquotes, parentheses and glob characters are not
# expanded, and the result is a single word that is not split.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.2.2-quoting.016
. "$PC_CORE/pc.sh"
pc_begin XCU.2.2-quoting.016
PCSH=${PC_SH:-sh}
LC_ALL=C
export PCSH LC_ALL
PCW=$PWD
PCD=pcw$$
trap 'cd "$PCW" 2>/dev/null; rm -rf "$PCD"' 0
mkdir "$PCD" && cd "$PCD" || pc_fail "cannot make a working directory"
{
	printf 'x=v\n'
	printf 'printf \047%%s\\n\047 $\047$x * ? `y` $(z) a  b\\tc\047\n'
} >s.sh || pc_fail "cannot write the test script"
: >pcqa || pc_fail "setup failed"
: >pcqb || pc_fail "setup failed"
pc_test
"$PCSH" ./s.sh </dev/null >out 2>err
rc=$?
want=$(printf '$x * ? `y` $(z) a  b\tc\n'; printf x)
got=$(cat out; printf x)
[ "$got" = "$want" ] || pc_fail "standard output was [$(cat out)]"
pc_pass
