# Inside double quotes the dollar sign still introduces parameter expansion,
# command substitution and arithmetic expansion, and the backquote still
# introduces the old form of command substitution.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.2.2-quoting.005
. "$PC_CORE/pc.sh"
pc_begin XCU.2.2-quoting.005
PCSH=${PC_SH:-sh}
LC_ALL=C
export PCSH LC_ALL
PCW=$PWD
PCD=pcw$$
trap 'cd "$PCW" 2>/dev/null; rm -rf "$PCD"' 0
mkdir "$PCD" && cd "$PCD" || pc_fail "cannot make a working directory"
{
	printf 'x=v\n'
	printf 'set -- p1 p2\n'
	printf 'printf \047%%s\\n\047 "$x ${x}w $1 $# $((1+2)) $(echo c) `echo d`"\n'
} >s.sh || pc_fail "cannot write the test script"
pc_test
"$PCSH" ./s.sh </dev/null >out 2>err
rc=$?
want=$(printf 'v vw p1 2 3 c d\n'; printf x)
got=$(cat out; printf x)
[ "$got" = "$want" ] || pc_fail "standard output was [$(cat out)]"
pc_pass
