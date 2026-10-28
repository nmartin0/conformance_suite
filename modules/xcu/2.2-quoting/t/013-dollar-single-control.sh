# In dollar-single-quotes, backslash-c followed by a letter yields the
# matching control character (A gives value 1, Z gives value 26), and
# backslash-c followed by two backslashes yields the FS control character
# (value 28).
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.2.2-quoting.013
. "$PC_CORE/pc.sh"
pc_begin XCU.2.2-quoting.013
PCSH=${PC_SH:-sh}
LC_ALL=C
export PCSH LC_ALL
PCW=$PWD
PCD=pcw$$
trap 'cd "$PCW" 2>/dev/null; rm -rf "$PCD"' 0
mkdir "$PCD" && cd "$PCD" || pc_fail "cannot make a working directory"
{
	printf 'printf \047%%s\\n\047 $\047\\cA\047 $\047\\cZ\047 $\047\\c\\\\\047\n'
} >s.sh || pc_fail "cannot write the test script"
pc_test
"$PCSH" ./s.sh </dev/null >out 2>err
rc=$?
want=$(printf '\001\n\032\n\034\n'; printf x)
got=$(cat out; printf x)
[ "$got" = "$want" ] || pc_fail "standard output was [$(cat out)]"
pc_pass
