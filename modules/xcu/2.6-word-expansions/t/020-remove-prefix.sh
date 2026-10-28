# ${p#w} removes the smallest prefix matching the pattern w and ${p##w} the
# largest; a value with no matching prefix, or an empty pattern, leaves p
# unchanged.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.2.6-word-expansions.020
. "$PC_CORE/pc.sh"
pc_begin XCU.2.6-word-expansions.020
PCSH=${PC_SH:-sh}
LC_ALL=C
export PCSH LC_ALL
PCW=$PWD
PCD=pcw$$
trap 'cd "$PCW" 2>/dev/null; rm -rf "$PCD"' 0
mkdir "$PCD" && cd "$PCD" || pc_fail "cannot make a working directory"
{
	printf 'x=/one/two/three\n'
	printf 'echo ${x#*/} ${x##*/} ${x#/} ${x##/*/}\n'
	printf 'x=abc\n'
	printf 'echo ${x#a*} ${x##a*}"|" ${x#?} ${x#[b-c]} ${x#} ${x#zz}\n'
} >s.sh || pc_fail "cannot write the test script"
pc_test
"$PCSH" ./s.sh </dev/null >out 2>err
rc=$?
want=$(printf 'one/two/three three one/two/three three\nbc | bc abc abc abc\n'; printf x)
got=$(cat out; printf x)
[ "$got" = "$want" ] || pc_fail "standard output was [$(cat out)]"
pc_pass
