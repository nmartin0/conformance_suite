# To find the end of a $( ) the shell tokenizes its text, so a closing
# parenthesis that is quoted, escaped, or the end of a case pattern does not
# end the substitution.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.2.3-token-recognition.007
. "$PC_CORE/pc.sh"
pc_begin XCU.2.3-token-recognition.007
PCSH=${PC_SH:-sh}
LC_ALL=C
export PCSH LC_ALL
PCW=$PWD
PCD=pcw$$
trap 'cd "$PCW" 2>/dev/null; rm -rf "$PCD"' 0
mkdir "$PCD" && cd "$PCD" || pc_fail "cannot make a working directory"
{
	printf 'echo $(case x in x) echo m;; esac)\n'
	printf 'echo $(echo "a)b")\n'
	printf 'echo $(echo \047c)d\047)\n'
	printf 'echo $(echo g\\)h)\n'
} >s.sh || pc_fail "cannot write the test script"
pc_test
"$PCSH" ./s.sh </dev/null >out 2>err
rc=$?
want=$(printf 'm\na)b\nc)d\ng)h\n'; printf x)
got=$(cat out; printf x)
[ "$got" = "$want" ] || pc_fail "standard output was [$(cat out)]"
pc_pass
