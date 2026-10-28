# The positional parameters can be reassigned with set and are temporarily
# replaced while a shell function runs, then restored.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.2.5-parameters.002
. "$PC_CORE/pc.sh"
pc_begin XCU.2.5-parameters.002
PCSH=${PC_SH:-sh}
LC_ALL=C
export PCSH LC_ALL
PCW=$PWD
PCD=pcw$$
trap 'cd "$PCW" 2>/dev/null; rm -rf "$PCD"' 0
mkdir "$PCD" && cd "$PCD" || pc_fail "cannot make a working directory"
{
	printf 'set -- a b c\n'
	printf 'f() { echo "$# $1"; set -- x; echo "$# $1"; }\n'
	printf 'f p q\n'
	printf 'echo "$# $1"\n'
	printf 'set -- m\n'
	printf 'echo "$# $1"\n'
} >s.sh || pc_fail "cannot write the test script"
pc_test
"$PCSH" ./s.sh </dev/null >out 2>err
rc=$?
want=$(printf '2 p\n1 x\n3 a\n1 m\n'; printf x)
got=$(cat out; printf x)
[ "$got" = "$want" ] || pc_fail "standard output was [$(cat out)]"
pc_pass
