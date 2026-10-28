# With <<- leading tab characters are removed from each line of the text and
# from the delimiter line (spaces are not), and tabs that come from
# expansions are kept.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.2.7-redirection.017
. "$PC_CORE/pc.sh"
pc_begin XCU.2.7-redirection.017
PCSH=${PC_SH:-sh}
LC_ALL=C
export PCSH LC_ALL
PCW=$PWD
PCD=pcw$$
trap 'cd "$PCW" 2>/dev/null; rm -rf "$PCD"' 0
mkdir "$PCD" && cd "$PCD" || pc_fail "cannot make a working directory"
{
	printf 'cat <<-E\n'
	printf '\ta\n'
	printf '\t\tb\n'
	printf ' \tc\n'
	printf 'd\n'
	printf '\tE\n'
	printf 't=$(printf \047\\t\047)\n'
	printf 'cat <<-E\n'
	printf '${t}e\n'
	printf 'E\n'
} >s.sh || pc_fail "cannot write the test script"
pc_test
"$PCSH" ./s.sh </dev/null >out 2>err
rc=$?
want=$(printf 'a\nb\n \tc\nd\n\te\n'; printf x)
got=$(cat out; printf x)
[ "$got" = "$want" ] || pc_fail "standard output was [$(cat out)]"
pc_pass
