# The brace that ends a ${ } expansion is the first one not escaped by a
# backslash, not inside quotes and not inside an embedded expansion or
# command substitution.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.2.6-word-expansions.022
. "$PC_CORE/pc.sh"
pc_begin XCU.2.6-word-expansions.022
PCSH=${PC_SH:-sh}
LC_ALL=C
export PCSH LC_ALL
PCW=$PWD
PCD=pcw$$
trap 'cd "$PCW" 2>/dev/null; rm -rf "$PCD"' 0
mkdir "$PCD" && cd "$PCD" || pc_fail "cannot make a working directory"
{
	printf 'foo=asdf\n'
	printf 'echo ${foo-bar}xyz}\n'
	printf 'foo=\n'
	printf 'echo ${foo-bar}xyz}\n'
	printf 'unset foo\n'
	printf 'echo ${foo-bar}xyz}\n'
	printf 'echo ${foo-a\\}b} ${foo-"}"} ${foo-$(echo \047}\047)} ${foo-\047}\047}\n'
} >s.sh || pc_fail "cannot write the test script"
pc_test
"$PCSH" ./s.sh </dev/null >out 2>err
rc=$?
want=$(printf 'asdfxyz}\nxyz}\nbarxyz}\na}b } } }\n'; printf x)
got=$(cat out; printf x)
[ "$got" = "$want" ] || pc_fail "standard output was [$(cat out)]"
pc_pass
