# When a command has several redirections they are carried out from first to
# last: with two output redirections both files are created and the last one
# is used; with two input redirections the last one is read.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.2.7-redirection.012
. "$PC_CORE/pc.sh"
pc_begin XCU.2.7-redirection.012
PCSH=${PC_SH:-sh}
LC_ALL=C
export PCSH LC_ALL
PCW=$PWD
PCD=pcw$$
trap 'cd "$PCW" 2>/dev/null; rm -rf "$PCD"' 0
mkdir "$PCD" && cd "$PCD" || pc_fail "cannot make a working directory"
{
	printf 'echo x >a1 >b1\n'
	printf 'if [ -f a1 ] && [ ! -s a1 ]; then echo a-empty; else echo a-bad; fi\n'
	printf 'cat b1\n'
	printf 'cat <a1 <b1\n'
} >s.sh || pc_fail "cannot write the test script"
pc_test
"$PCSH" ./s.sh </dev/null >out 2>err
rc=$?
want=$(printf 'a-empty\nx\nx\n'; printf x)
got=$(cat out; printf x)
[ "$got" = "$want" ] || pc_fail "standard output was [$(cat out)]"
pc_pass
