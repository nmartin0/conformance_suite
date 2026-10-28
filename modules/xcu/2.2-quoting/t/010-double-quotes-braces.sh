# Inside double quotes, in the text of a ${ } expansion such as ${u:-word},
# characters other than double quote, backquote, dollar sign and backslash
# keep their literal value (a single quote is literal), a backslash before a
# closing brace is an escape that hides it from the search for the end, and
# a nested $( ) has its own quoting.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.2.2-quoting.010
. "$PC_CORE/pc.sh"
pc_begin XCU.2.2-quoting.010
PCSH=${PC_SH:-sh}
LC_ALL=C
export PCSH LC_ALL
PCW=$PWD
PCD=pcw$$
trap 'cd "$PCW" 2>/dev/null; rm -rf "$PCD"' 0
mkdir "$PCD" && cd "$PCD" || pc_fail "cannot make a working directory"
{
	printf 'printf \047%%s\\n\047 "${u:-a*b|c d \047q\047 ~ #}"\n'
	printf 'printf \047%%s\\n\047 "${u:-\\$x \\\\ \\}z}"\n'
	printf 'printf \047%%s\\n\047 "${u:-$(echo ")")}"\n'
} >s.sh || pc_fail "cannot write the test script"
pc_test
"$PCSH" ./s.sh </dev/null >out 2>err
rc=$?
want=$(printf 'a*b|c d \047q\047 ~ #\n$x \\ }z\n)\n'; printf x)
got=$(cat out; printf x)
[ "$got" = "$want" ] || pc_fail "standard output was [$(cat out)]"
pc_pass
