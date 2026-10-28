# Inside double quotes the characters other than dollar sign, backquote and
# backslash keep their literal value: operators, glob characters, a single
# quote, a hash, a tilde, white space and newlines are not special, and no
# filename expansion happens.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.2.2-quoting.004
. "$PC_CORE/pc.sh"
pc_begin XCU.2.2-quoting.004
PCSH=${PC_SH:-sh}
LC_ALL=C
export PCSH LC_ALL
PCW=$PWD
PCD=pcw$$
trap 'cd "$PCW" 2>/dev/null; rm -rf "$PCD"' 0
mkdir "$PCD" && cd "$PCD" || pc_fail "cannot make a working directory"
{
	printf 'printf \047%%s\\n\047 "| & ; < > ( ) * ? [ ] \047 # ~ ! { } = %% ^ , pcq*" "a   b" "m\n'
	printf 'n"\n'
} >s.sh || pc_fail "cannot write the test script"
: >pcqa || pc_fail "setup failed"
: >pcqb || pc_fail "setup failed"
pc_test
"$PCSH" ./s.sh </dev/null >out 2>err
rc=$?
want=$(printf '| & ; < > ( ) * ? [ ] \047 # ~ ! { } = %% ^ , pcq*\na   b\nm\nn\n'; printf x)
got=$(cat out; printf x)
[ "$got" = "$want" ] || pc_fail "standard output was [$(cat out)]"
pc_pass
