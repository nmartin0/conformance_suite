# Where no field splitting follows, $* joins the positional parameters with
# the first character of IFS, with a space if IFS is unset, and with nothing
# if IFS is empty; this holds for "$*" and for an assignment.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.2.5-parameters.006
. "$PC_CORE/pc.sh"
pc_begin XCU.2.5-parameters.006
PCSH=${PC_SH:-sh}
LC_ALL=C
export PCSH LC_ALL
PCW=$PWD
PCD=pcw$$
trap 'cd "$PCW" 2>/dev/null; rm -rf "$PCD"' 0
mkdir "$PCD" && cd "$PCD" || pc_fail "cannot make a working directory"
{
	printf 'set -- a b c\n'
	printf 'IFS=:\n'
	printf 'echo "$*"\n'
	printf 'IFS=\047:;\047\n'
	printf 'echo "$*"\n'
	printf 'IFS=\n'
	printf 'echo "$*"\n'
	printf 'unset IFS\n'
	printf 'echo "$*"\n'
	printf 'x=$*\n'
	printf 'IFS=,\n'
	printf 'y=$*\n'
	printf 'echo "$x|$y"\n'
} >s.sh || pc_fail "cannot write the test script"
pc_test
"$PCSH" ./s.sh </dev/null >out 2>err
rc=$?
want=$(printf 'a:b:c\na:b:c\nabc\na b c\na b c|a,b,c\n'; printf x)
got=$(cat out; printf x)
[ "$got" = "$want" ] || pc_fail "standard output was [$(cat out)]"
pc_pass
