# The word after > and similar operators undergoes tilde expansion,
# parameter expansion, command substitution, arithmetic expansion and quote
# removal.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.2.7-redirection.010
. "$PC_CORE/pc.sh"
pc_begin XCU.2.7-redirection.010
PCSH=${PC_SH:-sh}
LC_ALL=C
export PCSH LC_ALL
PCW=$PWD
PCD=pcw$$
trap 'cd "$PCW" 2>/dev/null; rm -rf "$PCD"' 0
mkdir "$PCD" && cd "$PCD" || pc_fail "cannot make a working directory"
{
	printf 'f=tgt; echo p >$f\n'
	printf 'HOME=$PWD; echo q >~/tgt2\n'
	printf 'echo r >$(echo tgt3)\n'
	printf 'echo s >tgt$((1+3))\n'
	printf 'echo t >\047tg  t5\047\n'
	printf 'echo u >"tgt6"\n'
	printf 'cat tgt tgt2 tgt3 tgt4 \047tg  t5\047 tgt6\n'
} >s.sh || pc_fail "cannot write the test script"
pc_test
"$PCSH" ./s.sh </dev/null >out 2>err
rc=$?
want=$(printf 'p\nq\nr\ns\nt\nu\n'; printf x)
got=$(cat out; printf x)
[ "$got" = "$want" ] || pc_fail "standard output was [$(cat out)]"
pc_pass
