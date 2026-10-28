# A here-document can be attached to a descriptor number written before the
# operator.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.2.7-redirection.020
. "$PC_CORE/pc.sh"
pc_begin XCU.2.7-redirection.020
PCSH=${PC_SH:-sh}
LC_ALL=C
export PCSH LC_ALL
PCW=$PWD
PCD=pcw$$
trap 'cd "$PCW" 2>/dev/null; rm -rf "$PCD"' 0
mkdir "$PCD" && cd "$PCD" || pc_fail "cannot make a working directory"
{
	printf 'cat 3<<E <&3\n'
	printf 'hi\n'
	printf 'E\n'
	printf '{ cat <&3; } 3<<E\n'
	printf 'there\n'
	printf 'E\n'
} >s.sh || pc_fail "cannot write the test script"
pc_test
"$PCSH" ./s.sh </dev/null >out 2>err
rc=$?
want=$(printf 'hi\nthere\n'; printf x)
got=$(cat out; printf x)
[ "$got" = "$want" ] || pc_fail "standard output was [$(cat out)]"
pc_pass
