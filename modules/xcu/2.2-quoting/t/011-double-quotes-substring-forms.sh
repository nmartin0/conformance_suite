# For the four substring forms (# ## % %%) the enclosing double quotes have
# no effect on special characters inside the braces: pattern characters stay
# active, while quotes or a backslash inside the braces make a character
# literal.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.2.2-quoting.011
. "$PC_CORE/pc.sh"
pc_begin XCU.2.2-quoting.011
PCSH=${PC_SH:-sh}
LC_ALL=C
export PCSH LC_ALL
PCW=$PWD
PCD=pcw$$
trap 'cd "$PCW" 2>/dev/null; rm -rf "$PCD"' 0
mkdir "$PCD" && cd "$PCD" || pc_fail "cannot make a working directory"
{
	printf 'x=abc\n'
	printf 'printf \047%%s\\n\047 "${x#a*}" "${x#a\047*\047}" "${x##a*}"\n'
	printf 'y=\047a*c\047\n'
	printf 'printf \047%%s\\n\047 "${y#a\047*\047}" "${y#a\\*}"\n'
} >s.sh || pc_fail "cannot write the test script"
pc_test
"$PCSH" ./s.sh </dev/null >out 2>err
rc=$?
want=$(printf 'bc\nabc\n\nc\nc\n'; printf x)
got=$(cat out; printf x)
[ "$got" = "$want" ] || pc_fail "standard output was [$(cat out)]"
pc_pass
