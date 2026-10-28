# Field splitting comes before filename expansion: the unquoted result of an
# expansion is split into fields and each field that holds pattern
# characters is then matched against file names.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.2.6-word-expansions.002
. "$PC_CORE/pc.sh"
pc_begin XCU.2.6-word-expansions.002
PCSH=${PC_SH:-sh}
LC_ALL=C
export PCSH LC_ALL
PCW=$PWD
PCD=pcw$$
trap 'cd "$PCW" 2>/dev/null; rm -rf "$PCD"' 0
mkdir "$PCD" && cd "$PCD" || pc_fail "cannot make a working directory"
{
	printf 'x=\047pcg* pcz\047\n'
	printf 'printf \047[%%s]\\n\047 $x\n'
} >s.sh || pc_fail "cannot write the test script"
: >pcga || pc_fail "setup failed"
: >pcgb || pc_fail "setup failed"
pc_test
"$PCSH" ./s.sh </dev/null >out 2>err
rc=$?
want=$(printf '[pcga]\n[pcgb]\n[pcz]\n'; printf x)
got=$(cat out; printf x)
[ "$got" = "$want" ] || pc_fail "standard output was [$(cat out)]"
pc_pass
