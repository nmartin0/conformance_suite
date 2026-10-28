# When a redirection fails (a file that cannot be opened, a descriptor that
# is not open) the command is not run and the status is nonzero, and the
# shell goes on with the next command.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.2.7-redirection.013
. "$PC_CORE/pc.sh"
pc_begin XCU.2.7-redirection.013
PCSH=${PC_SH:-sh}
LC_ALL=C
export PCSH LC_ALL
PCW=$PWD
PCD=pcw$$
trap 'cd "$PCW" 2>/dev/null; rm -rf "$PCD"' 0
mkdir "$PCD" && cd "$PCD" || pc_fail "cannot make a working directory"
{
	printf 'if echo ran 2>/dev/null <nonexist; then echo A; else echo failed1; fi\n'
	printf 'if echo ran 2>/dev/null >nodir/f; then echo B; else echo failed2; fi\n'
	printf 'if echo ran 2>/dev/null <&6; then echo C; else echo failed3; fi\n'
	printf 'if echo ran 2>/dev/null >&6; then echo D; else echo failed4; fi\n'
	printf 'echo end\n'
} >s.sh || pc_fail "cannot write the test script"
pc_test
"$PCSH" ./s.sh </dev/null >out 2>err
rc=$?
want=$(printf 'failed1\nfailed2\nfailed3\nfailed4\nend\n'; printf x)
got=$(cat out; printf x)
[ "$got" = "$want" ] || pc_fail "standard output was [$(cat out)]"
pc_pass
