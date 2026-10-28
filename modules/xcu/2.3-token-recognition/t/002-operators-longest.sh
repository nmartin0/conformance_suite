# Characters that can be joined into a longer operator are read as that
# operator: >> appends rather than being two redirections, ;; ends a case
# item, && and || are the list operators, << starts a here-document and >&
# is one operator.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.2.3-token-recognition.002
. "$PC_CORE/pc.sh"
pc_begin XCU.2.3-token-recognition.002
PCSH=${PC_SH:-sh}
LC_ALL=C
export PCSH LC_ALL
PCW=$PWD
PCD=pcw$$
trap 'cd "$PCW" 2>/dev/null; rm -rf "$PCD"' 0
mkdir "$PCD" && cd "$PCD" || pc_fail "cannot make a working directory"
{
	printf 'echo one>x\n'
	printf 'echo two>>x\n'
	printf 'cat x\n'
	printf 'case a in a) echo m;; esac\n'
	printf 'true&&false||echo n\n'
	printf 'cat<<E\n'
	printf 'o\n'
	printf 'E\n'
	printf 'echo p 2>&1\n'
} >s.sh || pc_fail "cannot write the test script"
pc_test
"$PCSH" ./s.sh </dev/null >out 2>err
rc=$?
want=$(printf 'one\ntwo\nm\nn\no\np\n'; printf x)
got=$(cat out; printf x)
[ "$got" = "$want" ] || pc_fail "standard output was [$(cat out)]"
pc_pass
