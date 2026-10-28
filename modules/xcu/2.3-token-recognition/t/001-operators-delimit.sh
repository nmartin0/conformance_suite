# An operator character that is not quoted ends the token before it and
# starts an operator token, so operators need no blanks around them: ; && ||
# | ( ) and > all work when written tight against words, and a brace group
# closes with a semicolon written tight.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.2.3-token-recognition.001
. "$PC_CORE/pc.sh"
pc_begin XCU.2.3-token-recognition.001
PCSH=${PC_SH:-sh}
LC_ALL=C
export PCSH LC_ALL
PCW=$PWD
PCD=pcw$$
trap 'cd "$PCW" 2>/dev/null; rm -rf "$PCD"' 0
mkdir "$PCD" && cd "$PCD" || pc_fail "cannot make a working directory"
{
	printf 'echo a;echo b\n'
	printf 'true&&echo c\n'
	printf 'false||echo d\n'
	printf 'echo e|cat\n'
	printf '(echo f)\n'
	printf 'echo g>x;cat x\n'
	printf '{ echo h;}\n'
} >s.sh || pc_fail "cannot write the test script"
pc_test
"$PCSH" ./s.sh </dev/null >out 2>err
rc=$?
want=$(printf 'a\nb\nc\nd\ne\nf\ng\nh\n'; printf x)
got=$(cat out; printf x)
[ "$got" = "$want" ] || pc_fail "standard output was [$(cat out)]"
pc_pass
