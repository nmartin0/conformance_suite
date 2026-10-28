# With IFS set to the empty string no field splitting happens, but an
# unquoted expansion that gives an empty field is removed (unless quoting is
# also present in that field).
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.2.6-word-expansions.040
. "$PC_CORE/pc.sh"
pc_begin XCU.2.6-word-expansions.040
PCSH=${PC_SH:-sh}
LC_ALL=C
export PCSH LC_ALL
PCW=$PWD
PCD=pcw$$
trap 'cd "$PCW" 2>/dev/null; rm -rf "$PCD"' 0
mkdir "$PCD" && cd "$PCD" || pc_fail "cannot make a working directory"
{
	printf 'IFS=\n'
	printf 'x=\047a b\047; set -- $x; echo $#\n'
	printf 'x=; set -- $x; echo $#\n'
	printf 'set -- $x""; echo $#\n'
	printf 'set -- "$x"; echo $#\n'
	printf 'unset y; set -- $y; echo $#\n'
} >s.sh || pc_fail "cannot write the test script"
pc_test
"$PCSH" ./s.sh </dev/null >out 2>err
rc=$?
want=$(printf '1\n0\n1\n1\n0\n'; printf x)
got=$(cat out; printf x)
[ "$got" = "$want" ] || pc_fail "standard output was [$(cat out)]"
pc_pass
