# Filename expansion applies to unquoted fields with pattern characters
# (including those from expansions) and gives sorted matches, leaves a
# pattern that matches nothing as it was, is not done for quoted or escaped
# pattern characters, and is turned off by set -f.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.2.6-word-expansions.003
. "$PC_CORE/pc.sh"
pc_begin XCU.2.6-word-expansions.003
PCSH=${PC_SH:-sh}
LC_ALL=C
export PCSH LC_ALL
PCW=$PWD
PCD=pcw$$
trap 'cd "$PCW" 2>/dev/null; rm -rf "$PCD"' 0
mkdir "$PCD" && cd "$PCD" || pc_fail "cannot make a working directory"
{
	printf 'x=\047pcg*\047\n'
	printf 'echo $x\n'
	printf 'echo "$x" \047$x\047 pcg\\*\n'
	printf 'set -f\n'
	printf 'echo $x pcg*\n'
	printf 'set +f\n'
	printf 'echo pcg*\n'
	printf 'echo pcnone*\n'
} >s.sh || pc_fail "cannot write the test script"
: >pcga || pc_fail "setup failed"
: >pcgb || pc_fail "setup failed"
pc_test
"$PCSH" ./s.sh </dev/null >out 2>err
rc=$?
want=$(printf 'pcga pcgb\npcg* $x pcg*\npcg* pcg*\npcga pcgb\npcnone*\n'; printf x)
got=$(cat out; printf x)
[ "$got" = "$want" ] || pc_fail "standard output was [$(cat out)]"
pc_pass
