# Redirections can name any descriptor from 0 to 9: a file opened on each of
# 3 to 9 and written through that number receives the data.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.2.7-redirection.003
. "$PC_CORE/pc.sh"
pc_begin XCU.2.7-redirection.003
PCSH=${PC_SH:-sh}
LC_ALL=C
export PCSH LC_ALL
PCW=$PWD
PCD=pcw$$
trap 'cd "$PCW" 2>/dev/null; rm -rf "$PCD"' 0
mkdir "$PCD" && cd "$PCD" || pc_fail "cannot make a working directory"
{
	printf 'echo fd3 3>f3 >&3\n'
	printf 'echo fd4 4>f4 >&4\n'
	printf 'echo fd5 5>f5 >&5\n'
	printf 'echo fd6 6>f6 >&6\n'
	printf 'echo fd7 7>f7 >&7\n'
	printf 'echo fd8 8>f8 >&8\n'
	printf 'echo fd9 9>f9 >&9\n'
	printf 'cat f3 f4 f5 f6 f7 f8 f9\n'
} >s.sh || pc_fail "cannot write the test script"
pc_test
"$PCSH" ./s.sh </dev/null >out 2>err
rc=$?
want=$(printf 'fd3\nfd4\nfd5\nfd6\nfd7\nfd8\nfd9\n'; printf x)
got=$(cat out; printf x)
[ "$got" = "$want" ] || pc_fail "standard output was [$(cat out)]"
pc_pass
