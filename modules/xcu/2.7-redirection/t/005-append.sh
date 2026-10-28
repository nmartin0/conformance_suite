# The >> operator opens the named file for appending, so each write goes to
# the end, and creates it when absent.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.2.7-redirection.005
. "$PC_CORE/pc.sh"
pc_begin XCU.2.7-redirection.005
PCSH=${PC_SH:-sh}
LC_ALL=C
export PCSH LC_ALL
PCW=$PWD
PCD=pcw$$
trap 'cd "$PCW" 2>/dev/null; rm -rf "$PCD"' 0
mkdir "$PCD" && cd "$PCD" || pc_fail "cannot make a working directory"
{
	printf 'echo a >f\n'
	printf 'echo b >>f\n'
	printf 'echo c >>f\n'
	printf 'cat f\n'
	printf 'echo n >>newfile\n'
	printf 'cat newfile\n'
} >s.sh || pc_fail "cannot write the test script"
pc_test
"$PCSH" ./s.sh </dev/null >out 2>err
rc=$?
want=$(printf 'a\nb\nc\nn\n'; printf x)
got=$(cat out; printf x)
[ "$got" = "$want" ] || pc_fail "standard output was [$(cat out)]"
pc_pass
