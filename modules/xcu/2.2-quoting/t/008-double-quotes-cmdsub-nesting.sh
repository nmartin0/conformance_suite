# The commands inside a $( ) that sits within double quotes are not affected
# by those double quotes: they can hold their own quotes, and a quoted
# parenthesis does not end the substitution.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.2.2-quoting.008
. "$PC_CORE/pc.sh"
pc_begin XCU.2.2-quoting.008
PCSH=${PC_SH:-sh}
LC_ALL=C
export PCSH LC_ALL
PCW=$PWD
PCD=pcw$$
trap 'cd "$PCW" 2>/dev/null; rm -rf "$PCD"' 0
mkdir "$PCD" && cd "$PCD" || pc_fail "cannot make a working directory"
{
	printf 'printf \047%%s\\n\047 "$(echo "a b")" "$(printf \047%%s\047 \047)\047 "(")" "x$(echo \047q"r\047)y"\n'
} >s.sh || pc_fail "cannot write the test script"
pc_test
"$PCSH" ./s.sh </dev/null >out 2>err
rc=$?
want=$(printf 'a b\n)(\nxq"ry\n'; printf x)
got=$(cat out; printf x)
[ "$got" = "$want" ] || pc_fail "standard output was [$(cat out)]"
pc_pass
