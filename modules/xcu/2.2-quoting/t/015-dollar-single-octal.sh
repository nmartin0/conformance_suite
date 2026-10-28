# In dollar-single-quotes, a backslash followed by one to three octal digits
# yields the byte with that value; the sequence ends after three digits or
# at the first character that is not an octal digit.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.2.2-quoting.015
. "$PC_CORE/pc.sh"
pc_begin XCU.2.2-quoting.015
PCSH=${PC_SH:-sh}
LC_ALL=C
export PCSH LC_ALL
PCW=$PWD
PCD=pcw$$
trap 'cd "$PCW" 2>/dev/null; rm -rf "$PCD"' 0
mkdir "$PCD" && cd "$PCD" || pc_fail "cannot make a working directory"
{
	printf 'printf \047%%s\\n\047 $\047\\101\047 $\047\\1010\047 $\047\\7z\047 $\047\\0101\047 $\047\\60\\061\047\n'
} >s.sh || pc_fail "cannot write the test script"
pc_test
"$PCSH" ./s.sh </dev/null >out 2>err
rc=$?
want=$(printf 'A\nA0\n\007z\n\0101\n01\n'; printf x)
got=$(cat out; printf x)
[ "$got" = "$want" ] || pc_fail "standard output was [$(cat out)]"
pc_pass
