# The word of a ${ } modifier undergoes tilde expansion, parameter
# expansion, command substitution, arithmetic expansion and quote removal.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.2.6-word-expansions.016
. "$PC_CORE/pc.sh"
pc_begin XCU.2.6-word-expansions.016
PCSH=${PC_SH:-sh}
LC_ALL=C
export PCSH LC_ALL
PCW=$PWD
PCD=pcw$$
trap 'cd "$PCW" 2>/dev/null; rm -rf "$PCD"' 0
mkdir "$PCD" && cd "$PCD" || pc_fail "cannot make a working directory"
{
	printf 'HOME=/h\n'
	printf 'unset u\n'
	printf 'a=${u:-~}; b=${u:-$HOME/q}; c=${u:-$(echo cs)}; d=${u:-$((2+3))}; e=${u:-\047c  d\047}\n'
	printf 'echo "$a $b $c $d [$e]"\n'
} >s.sh || pc_fail "cannot write the test script"
pc_test
"$PCSH" ./s.sh </dev/null >out 2>err
rc=$?
want=$(printf '/h /h/q cs 5 [c  d]\n'; printf x)
got=$(cat out; printf x)
[ "$got" = "$want" ] || pc_fail "standard output was [$(cat out)]"
pc_pass
