# When an execution trace is on (set -x), the value of PS4 undergoes
# parameter expansion and is written to standard error before each traced
# command.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.2.5-parameters.019
. "$PC_CORE/pc.sh"
pc_begin XCU.2.5-parameters.019
PCSH=${PC_SH:-sh}
LC_ALL=C
export PCSH LC_ALL
PCW=$PWD
PCD=pcw$$
trap 'cd "$PCW" 2>/dev/null; rm -rf "$PCD"' 0
mkdir "$PCD" && cd "$PCD" || pc_fail "cannot make a working directory"
{
	printf 'v=2\n'
	printf 'PS4=\047pc${v}> \047\n'
	printf 'set -x\n'
	printf 'echo hi\n'
	printf 'set +x\n'
} >s.sh || pc_fail "cannot write the test script"
pc_test
"$PCSH" ./s.sh </dev/null >out 2>err
rc=$?
want=$(printf 'hi\n'; printf x)
got=$(cat out; printf x)
[ "$got" = "$want" ] || pc_fail "standard output was [$(cat out)]"
case $(cat err) in "pc2> "*) ;; *) pc_fail "the trace did not start with the expanded PS4";; esac
pc_pass
