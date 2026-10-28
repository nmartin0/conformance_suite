# If any part of the delimiter word is quoted, the delimiter is the word
# with quote removal and the lines of the here-document are used as they
# are, with no expansion and backslash-newline kept.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.2.7-redirection.016
. "$PC_CORE/pc.sh"
pc_begin XCU.2.7-redirection.016
PCSH=${PC_SH:-sh}
LC_ALL=C
export PCSH LC_ALL
PCW=$PWD
PCD=pcw$$
trap 'cd "$PCW" 2>/dev/null; rm -rf "$PCD"' 0
mkdir "$PCD" && cd "$PCD" || pc_fail "cannot make a working directory"
{
	printf 'x=v\n'
	printf 'cat <<\047E\047\n'
	printf '$x $(echo c) `d` \\$ \\\\ a\\\n'
	printf 'b\n'
	printf 'E\n'
	printf 'cat <<\\E\n'
	printf '$x\n'
	printf 'E\n'
	printf 'cat <<"E"\n'
	printf '$x\n'
	printf 'E\n'
	printf 'cat <<E""\n'
	printf '$x\n'
	printf 'E\n'
} >s.sh || pc_fail "cannot write the test script"
pc_test
"$PCSH" ./s.sh </dev/null >out 2>err
rc=$?
want=$(printf '$x $(echo c) `d` \\$ \\\\ a\\\nb\n$x\n$x\n$x\n'; printf x)
got=$(cat out; printf x)
[ "$got" = "$want" ] || pc_fail "standard output was [$(cat out)]"
pc_pass
