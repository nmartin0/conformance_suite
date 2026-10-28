# A word containing any quoting is not replaced by an alias, and alias
# definitions are not inherited by a separate invocation of the shell.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.2.3-token-recognition.013
. "$PC_CORE/pc.sh"
pc_begin XCU.2.3-token-recognition.013
PCSH=${PC_SH:-sh}
LC_ALL=C
export PCSH LC_ALL
PCW=$PWD
PCD=pcw$$
trap 'cd "$PCW" 2>/dev/null; rm -rf "$PCD"' 0
mkdir "$PCD" && cd "$PCD" || pc_fail "cannot make a working directory"
{
	printf 'alias pcal6=\047echo Z\047\n'
	printf '(\047pcal6\047 hi) 2>/dev/null\n'
	printf '(\\pcal6 hi) 2>/dev/null\n'
	printf '("pcal6" hi) 2>/dev/null\n'
	printf '"$PCSH" -c \047pcal6 hi\047 2>/dev/null\n'
	printf 'echo end\n'
} >s.sh || pc_fail "cannot write the test script"
pc_test
"$PCSH" ./s.sh </dev/null >out 2>err
rc=$?
want=$(printf 'end\n'; printf x)
got=$(cat out; printf x)
[ "$got" = "$want" ] || pc_fail "standard output was [$(cat out)]"
pc_pass
