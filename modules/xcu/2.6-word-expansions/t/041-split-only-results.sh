# Only the results of unquoted expansions are split: delimiter characters
# written literally in the word, or coming from a quoted expansion, do not
# split it.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.2.6-word-expansions.041
. "$PC_CORE/pc.sh"
pc_begin XCU.2.6-word-expansions.041
PCSH=${PC_SH:-sh}
LC_ALL=C
export PCSH LC_ALL
PCW=$PWD
PCD=pcw$$
trap 'cd "$PCW" 2>/dev/null; rm -rf "$PCD"' 0
mkdir "$PCD" && cd "$PCD" || pc_fail "cannot make a working directory"
{
	printf 'IFS=:\n'
	printf 'set -- a:b; echo $#\n'
	printf 'x=c:d\n'
	printf 'set -- a:b$x; echo $# "$1" "$2"\n'
	printf 'set -- "$x"; echo $#\n'
	printf 'set -- $x"$x"; echo $# "$1" "$2"\n'
	printf 'set -- $(echo p:q) $((1)):z; echo $#\n'
} >s.sh || pc_fail "cannot write the test script"
pc_test
"$PCSH" ./s.sh </dev/null >out 2>err
rc=$?
want=$(printf '1\n2 a:bc d\n1\n2 c dc:d\n3\n'; printf x)
got=$(cat out; printf x)
[ "$got" = "$want" ] || pc_fail "standard output was [$(cat out)]"
pc_pass
