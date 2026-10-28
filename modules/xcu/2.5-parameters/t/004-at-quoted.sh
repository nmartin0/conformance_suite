# Within double quotes "$@" yields one field per positional parameter, empty
# ones included, none at all when there are none, and when embedded in a
# word the first field joins the text before it and the last field the text
# after it.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.2.5-parameters.004
. "$PC_CORE/pc.sh"
pc_begin XCU.2.5-parameters.004
PCSH=${PC_SH:-sh}
LC_ALL=C
export PCSH LC_ALL
PCW=$PWD
PCD=pcw$$
trap 'cd "$PCW" 2>/dev/null; rm -rf "$PCD"' 0
mkdir "$PCD" && cd "$PCD" || pc_fail "cannot make a working directory"
{
	printf 'set -- "a b" c ""\n'
	printf 'printf \047[%%s]\\n\047 "$@"\n'
	printf 'set --\n'
	printf 'set x "$@" y\n'
	printf 'echo $#\n'
	printf 'set -- a b\n'
	printf 'printf \047[%%s]\\n\047 "x$@y"\n'
	printf 'printf \047[%%s]\\n\047 "$@"x\n'
} >s.sh || pc_fail "cannot write the test script"
pc_test
"$PCSH" ./s.sh </dev/null >out 2>err
rc=$?
want=$(printf '[a b]\n[c]\n[]\n2\n[xa]\n[by]\n[a]\n[bx]\n'; printf x)
got=$(cat out; printf x)
[ "$got" = "$want" ] || pc_fail "standard output was [$(cat out)]"
pc_pass
