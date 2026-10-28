# The >& operator with a descriptor number makes the left descriptor
# (standard output by default) a copy of that open descriptor, with - closes
# it (closing a descriptor that is not open is no error), and redirections
# are processed from left to right so the order of 2>&1 and a file
# redirection matters.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.2.7-redirection.008
. "$PC_CORE/pc.sh"
pc_begin XCU.2.7-redirection.008
PCSH=${PC_SH:-sh}
LC_ALL=C
export PCSH LC_ALL
PCW=$PWD
PCD=pcw$$
trap 'cd "$PCW" 2>/dev/null; rm -rf "$PCD"' 0
mkdir "$PCD" && cd "$PCD" || pc_fail "cannot make a working directory"
{
	printf '{ echo out; echo err >&2; } >f 2>&1\n'
	printf 'cat f\n'
	printf '{ echo out2; echo err2 >&2; } 2>&1 >g\n'
	printf 'cat g\n'
	printf 'exec 3>h\n'
	printf '"$PCSH" -c \047echo leak >&3\047 2>/dev/null 3>&-\n'
	printf 'exec 3>&-\n'
	printf 'if [ -s h ]; then echo leaked; else echo closed; fi\n'
	printf 'exec 7>&-\n'
	printf 'echo not-an-error\n'
} >s.sh || pc_fail "cannot write the test script"
pc_test
"$PCSH" ./s.sh </dev/null >out 2>err
rc=$?
want=$(printf 'out\nerr\nerr2\nout2\nclosed\nnot-an-error\n'; printf x)
got=$(cat out; printf x)
[ "$got" = "$want" ] || pc_fail "standard output was [$(cat out)]"
pc_pass
