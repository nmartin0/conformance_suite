# After a here-document operator the rest of that line is read as ordinary
# tokens and kept; the body is taken from the lines after the next newline,
# and the saved tokens are then run; with two operators on a line the first
# body comes first.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.2.3-token-recognition.009
. "$PC_CORE/pc.sh"
pc_begin XCU.2.3-token-recognition.009
PCSH=${PC_SH:-sh}
LC_ALL=C
export PCSH LC_ALL
PCW=$PWD
PCD=pcw$$
trap 'cd "$PCW" 2>/dev/null; rm -rf "$PCD"' 0
mkdir "$PCD" && cd "$PCD" || pc_fail "cannot make a working directory"
{
	printf 'cat <<A; echo after1\n'
	printf 'body1\n'
	printf 'A\n'
	printf 'cat <<B <<C\n'
	printf 'b2\n'
	printf 'B\n'
	printf 'c3\n'
	printf 'C\n'
	printf 'echo after3\n'
} >s.sh || pc_fail "cannot write the test script"
pc_test
"$PCSH" ./s.sh </dev/null >out 2>err
rc=$?
want=$(printf 'body1\nafter1\nc3\nafter3\n'; printf x)
got=$(cat out; printf x)
[ "$got" = "$want" ] || pc_fail "standard output was [$(cat out)]"
pc_pass
