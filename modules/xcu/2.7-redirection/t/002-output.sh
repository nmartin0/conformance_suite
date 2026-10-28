# The > operator opens the named file for writing on standard output (or the
# descriptor written before it), creating it when absent and emptying it
# when present, and the redirection stays in effect for a descriptor opened
# with exec.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.2.7-redirection.002
. "$PC_CORE/pc.sh"
pc_begin XCU.2.7-redirection.002
PCSH=${PC_SH:-sh}
LC_ALL=C
export PCSH LC_ALL
PCW=$PWD
PCD=pcw$$
trap 'cd "$PCW" 2>/dev/null; rm -rf "$PCD"' 0
mkdir "$PCD" && cd "$PCD" || pc_fail "cannot make a working directory"
{
	printf 'echo one >f\n'
	printf 'echo two >f\n'
	printf 'cat f\n'
	printf 'rm -f g\n'
	printf 'echo x >g\n'
	printf 'cat g\n'
	printf '{ echo err >&2; } 2>e\n'
	printf 'cat e\n'
	printf 'exec 9>nine\n'
	printf 'echo nine >&9\n'
	printf 'exec 9>&-\n'
	printf 'cat nine\n'
} >s.sh || pc_fail "cannot write the test script"
pc_test
"$PCSH" ./s.sh </dev/null >out 2>err
rc=$?
want=$(printf 'two\nx\nerr\nnine\n'; printf x)
got=$(cat out; printf x)
[ "$got" = "$want" ] || pc_fail "standard output was [$(cat out)]"
pc_pass
