# The special parameter ? expands to the decimal exit status of the most
# recent pipeline executed in the current shell environment (the last
# command of it), starting at 0, set by failure, by a pipeline's last
# command, by negation, and by an assignment's command substitution, and a
# subshell starts with the value of its parent.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.2.5-parameters.007
. "$PC_CORE/pc.sh"
pc_begin XCU.2.5-parameters.007
PCSH=${PC_SH:-sh}
LC_ALL=C
export PCSH LC_ALL
PCW=$PWD
PCD=pcw$$
trap 'cd "$PCW" 2>/dev/null; rm -rf "$PCD"' 0
mkdir "$PCD" && cd "$PCD" || pc_fail "cannot make a working directory"
{
	printf 'echo $?\n'
	printf 'false\n'
	printf 'echo $?\n'
	printf 'true\n'
	printf 'echo $?\n'
	printf '(exit 3)\n'
	printf 'echo $?\n'
	printf 'false | true\n'
	printf 'echo $?\n'
	printf 'true | false\n'
	printf 'echo $?\n'
	printf '! true\n'
	printf 'echo $?\n'
	printf 'x=$(exit 4)\n'
	printf 'echo $?\n'
	printf 'false\n'
	printf '(echo $?)\n'
} >s.sh || pc_fail "cannot write the test script"
pc_test
"$PCSH" ./s.sh </dev/null >out 2>err
rc=$?
want=$(printf '0\n1\n0\n3\n0\n1\n1\n4\n1\n'; printf x)
got=$(cat out; printf x)
[ "$got" = "$want" ] || pc_fail "standard output was [$(cat out)]"
pc_pass
