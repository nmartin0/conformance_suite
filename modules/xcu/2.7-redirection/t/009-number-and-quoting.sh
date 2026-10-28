# A descriptor number must stand directly before the operator and be
# unquoted; a quoted number, a quoted operator, or a number set apart by a
# blank is not part of the redirection, and the redirection operator and its
# word do not reach the command as arguments.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.2.7-redirection.009
. "$PC_CORE/pc.sh"
pc_begin XCU.2.7-redirection.009
PCSH=${PC_SH:-sh}
LC_ALL=C
export PCSH LC_ALL
PCW=$PWD
PCD=pcw$$
trap 'cd "$PCW" 2>/dev/null; rm -rf "$PCD"' 0
mkdir "$PCD" && cd "$PCD" || pc_fail "cannot make a working directory"
{
	printf 'echo \\2>f\n'
	printf 'cat f\n'
	printf 'echo 2\\>f\n'
	printf 'printf \047%%s\\n\047 a >g b\n'
	printf 'cat g\n'
	printf 'echo 2 >h\n'
	printf 'cat h\n'
} >s.sh || pc_fail "cannot write the test script"
pc_test
"$PCSH" ./s.sh </dev/null >out 2>err
rc=$?
want=$(printf '2\n2>f\na\nb\n2\n'; printf x)
got=$(cat out; printf x)
[ "$got" = "$want" ] || pc_fail "standard output was [$(cat out)]"
pc_pass
