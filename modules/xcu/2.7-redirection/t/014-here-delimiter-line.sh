# A here-document ends at the first line that holds only the delimiter: a
# line with the delimiter and a trailing blank, or with a leading blank, is
# part of the text.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.2.7-redirection.014
. "$PC_CORE/pc.sh"
pc_begin XCU.2.7-redirection.014
PCSH=${PC_SH:-sh}
LC_ALL=C
export PCSH LC_ALL
PCW=$PWD
PCD=pcw$$
trap 'cd "$PCW" 2>/dev/null; rm -rf "$PCD"' 0
mkdir "$PCD" && cd "$PCD" || pc_fail "cannot make a working directory"
{
	printf 'cat <<E\n'
	printf 'a\n'
	printf 'E\n'
	printf 'cat <<E\n'
	printf 'x\n'
	printf 'E \n'
	printf 'y\n'
	printf ' E\n'
	printf 'E\n'
	printf 'echo end\n'
} >s.sh || pc_fail "cannot write the test script"
pc_test
"$PCSH" ./s.sh </dev/null >out 2>err
rc=$?
want=$(printf 'a\nx\nE \ny\n E\nend\n'; printf x)
got=$(cat out; printf x)
[ "$got" = "$want" ] || pc_fail "standard output was [$(cat out)]"
pc_pass
