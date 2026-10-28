# In a variable assignment a tilde-prefix is also recognized after the
# equals sign and after each unquoted colon, and ends at the next colon or
# slash or at the end of the word.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.2.6-word-expansions.008
. "$PC_CORE/pc.sh"
pc_begin XCU.2.6-word-expansions.008
PCSH=${PC_SH:-sh}
LC_ALL=C
export PCSH LC_ALL
PCW=$PWD
PCD=pcw$$
trap 'cd "$PCW" 2>/dev/null; rm -rf "$PCD"' 0
mkdir "$PCD" && cd "$PCD" || pc_fail "cannot make a working directory"
{
	printf 'HOME=/h\n'
	printf 'v=~/a:~/b:c\n'
	printf 'echo "$v"\n'
	printf 'w=~\n'
	printf 'echo "$w"\n'
	printf 'u=x:~\n'
	printf 'echo "$u"\n'
} >s.sh || pc_fail "cannot write the test script"
pc_test
"$PCSH" ./s.sh </dev/null >out 2>err
rc=$?
want=$(printf '/h/a:/h/b:c\n/h\nx:/h\n'; printf x)
got=$(cat out; printf x)
[ "$got" = "$want" ] || pc_fail "standard output was [$(cat out)]"
pc_pass
