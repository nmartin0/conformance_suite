# A command substitution is replaced by the standard output of its commands
# in both forms, $( ) and backquotes, with every trailing newline removed
# and all other newlines kept.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.2.6-word-expansions.023
. "$PC_CORE/pc.sh"
pc_begin XCU.2.6-word-expansions.023
PCSH=${PC_SH:-sh}
LC_ALL=C
export PCSH LC_ALL
PCW=$PWD
PCD=pcw$$
trap 'cd "$PCW" 2>/dev/null; rm -rf "$PCD"' 0
mkdir "$PCD" && cd "$PCD" || pc_fail "cannot make a working directory"
{
	printf 'echo $(echo hi) `echo ho`\n'
	printf 'x=$(printf \047a\\n\\n\\n\047); echo "[$x]"\n'
	printf 'x=$(printf \047\\n\\nb\\nc\\n\\n\047); echo "[$x]"\n'
	printf 'x=$(printf \047tail  \047); echo "[$x]"\n'
	printf 'x=$(printf \047\\n\\n\\n\047); echo "[$x]"\n'
} >s.sh || pc_fail "cannot write the test script"
pc_test
"$PCSH" ./s.sh </dev/null >out 2>err
rc=$?
want=$(printf 'hi ho\n[a]\n[\n\nb\nc]\n[tail  ]\n[]\n'; printf x)
got=$(cat out; printf x)
[ "$got" = "$want" ] || pc_fail "standard output was [$(cat out)]"
pc_pass
