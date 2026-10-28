# With an unquoted delimiter, backslash-newline is removed while the
# delimiter line is looked for, so a delimiter that line continuation has
# glued to the line before it is not recognized.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.2.7-redirection.023
. "$PC_CORE/pc.sh"
pc_begin XCU.2.7-redirection.023
PCSH=${PC_SH:-sh}
LC_ALL=C
export PCSH LC_ALL
PCW=$PWD
PCD=pcw$$
trap 'cd "$PCW" 2>/dev/null; rm -rf "$PCD"' 0
mkdir "$PCD" && cd "$PCD" || pc_fail "cannot make a working directory"
{
	printf 'cat <<E\n'
	printf 'a\\\n'
	printf 'E\n'
	printf 'b\n'
	printf 'E\n'
} >s.sh || pc_fail "cannot write the test script"
pc_test
"$PCSH" ./s.sh </dev/null >out 2>err
rc=$?
want=$(printf 'aE\nb\n'; printf x)
got=$(cat out; printf x)
[ "$got" = "$want" ] || pc_fail "standard output was [$(cat out)]"
pc_pass
