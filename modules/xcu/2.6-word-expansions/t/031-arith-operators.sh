# Arithmetic expansion has the C shift, relational, equality, bitwise and
# logical operators, which yield 1 or 0 for the relational, equality and
# logical ones.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.2.6-word-expansions.031
. "$PC_CORE/pc.sh"
pc_begin XCU.2.6-word-expansions.031
PCSH=${PC_SH:-sh}
LC_ALL=C
export PCSH LC_ALL
PCW=$PWD
PCD=pcw$$
trap 'cd "$PCW" 2>/dev/null; rm -rf "$PCD"' 0
mkdir "$PCD" && cd "$PCD" || pc_fail "cannot make a working directory"
{
	printf 'echo $((1<<4)) $((256>>2)) $((3<4)) $((4<=3)) $((5>4)) $((4>=5)) $((3==3)) $((3!=3))\n'
	printf 'echo $((12&10)) $((12|10)) $((12^10)) $((~5&7)) $((!5)) $((!0)) $((1&&0)) $((0||5)) $((2&&3))\n'
} >s.sh || pc_fail "cannot write the test script"
pc_test
"$PCSH" ./s.sh </dev/null >out 2>err
rc=$?
want=$(printf '16 64 1 0 1 0 1 0\n8 14 6 2 0 1 0 1 1\n'; printf x)
got=$(cat out; printf x)
[ "$got" = "$want" ] || pc_fail "standard output was [$(cat out)]"
pc_pass
