# The <& operator with a descriptor number makes the left descriptor
# (standard input by default) a copy of that open descriptor, and with -
# closes it; closing a descriptor that is not open is not an error.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.2.7-redirection.007
. "$PC_CORE/pc.sh"
pc_begin XCU.2.7-redirection.007
PCSH=${PC_SH:-sh}
LC_ALL=C
export PCSH LC_ALL
PCW=$PWD
PCD=pcw$$
trap 'cd "$PCW" 2>/dev/null; rm -rf "$PCD"' 0
mkdir "$PCD" && cd "$PCD" || pc_fail "cannot make a working directory"
{
	printf 'printf \047%%s\\n\047 d1 d2 >f\n'
	printf 'cat 3<f <&3\n'
	printf 'exec 3<f 5<&3\n'
	printf 'exec 3<&-\n'
	printf 'cat <&5\n'
	printf 'exec 5<&-\n'
	printf 'exec 7<&-\n'
	printf 'echo not-an-error\n'
	printf '"$PCSH" -c \047cat <&3\047 3<f 3<&- 2>/dev/null\n'
	printf 'echo end\n'
} >s.sh || pc_fail "cannot write the test script"
pc_test
"$PCSH" ./s.sh </dev/null >out 2>err
rc=$?
want=$(printf 'd1\nd2\nd1\nd2\nnot-an-error\nend\n'; printf x)
got=$(cat out; printf x)
[ "$got" = "$want" ] || pc_fail "standard output was [$(cat out)]"
pc_pass
