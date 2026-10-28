# ${p:-w} gives w when p is unset or null and ${p-w} only when p is unset;
# otherwise both give the value of p.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.2.6-word-expansions.011
. "$PC_CORE/pc.sh"
pc_begin XCU.2.6-word-expansions.011
PCSH=${PC_SH:-sh}
LC_ALL=C
export PCSH LC_ALL
PCW=$PWD
PCD=pcw$$
trap 'cd "$PCW" 2>/dev/null; rm -rf "$PCD"' 0
mkdir "$PCD" && cd "$PCD" || pc_fail "cannot make a working directory"
{
	printf 'x=v; echo "${x:-w}" "${x-w}"\n'
	printf 'x=; echo "[${x:-w}]" "[${x-w}]"\n'
	printf 'unset x; echo "[${x:-w}]" "[${x-w}]"\n'
	printf 'set -- \047\047; echo "${1:-d}|${1-d}"\n'
} >s.sh || pc_fail "cannot write the test script"
pc_test
"$PCSH" ./s.sh </dev/null >out 2>err
rc=$?
want=$(printf 'v v\n[w] []\n[w] [w]\nd|\n'; printf x)
got=$(cat out; printf x)
[ "$got" = "$want" ] || pc_fail "standard output was [$(cat out)]"
pc_pass
