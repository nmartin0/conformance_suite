# A backslash followed by a newline is removed entirely before the input is
# split into tokens, so it joins the text on both sides and does not
# separate tokens; a blank written after the removed pair still does.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.2.2-quoting.002
. "$PC_CORE/pc.sh"
pc_begin XCU.2.2-quoting.002
PCSH=${PC_SH:-sh}
LC_ALL=C
export PCSH LC_ALL
PCW=$PWD
PCD=pcw$$
trap 'cd "$PCW" 2>/dev/null; rm -rf "$PCD"' 0
mkdir "$PCD" && cd "$PCD" || pc_fail "cannot make a working directory"
{
	printf 'echo a\\\n'
	printf 'b\n'
	printf 'ec\\\n'
	printf 'ho c\n'
	printf 'printf \047%%s\\n\047 x\\\n'
	printf ' y\n'
} >s.sh || pc_fail "cannot write the test script"
pc_test
"$PCSH" ./s.sh </dev/null >out 2>err
rc=$?
want=$(printf 'ab\nc\nx\ny\n'; printf x)
got=$(cat out; printf x)
[ "$got" = "$want" ] || pc_fail "standard output was [$(cat out)]"
pc_pass
