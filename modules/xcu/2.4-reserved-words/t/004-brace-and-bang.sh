# The words { and } make a grouping command and ! is a reserved word that
# negates the status of the pipeline after it, when they appear in command
# position.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.2.4-reserved-words.004
. "$PC_CORE/pc.sh"
pc_begin XCU.2.4-reserved-words.004
PCSH=${PC_SH:-sh}
LC_ALL=C
export PCSH LC_ALL
PCW=$PWD
PCD=pcw$$
trap 'cd "$PCW" 2>/dev/null; rm -rf "$PCD"' 0
mkdir "$PCD" && cd "$PCD" || pc_fail "cannot make a working directory"
{
	printf '{ echo g1; echo g2; }\n'
	printf '! false && echo n1\n'
	printf '! true || echo n2\n'
} >s.sh || pc_fail "cannot write the test script"
pc_test
"$PCSH" ./s.sh </dev/null >out 2>err
rc=$?
want=$(printf 'g1\ng2\nn1\nn2\n'; printf x)
got=$(cat out; printf x)
[ "$got" = "$want" ] || pc_fail "standard output was [$(cat out)]"
pc_pass
