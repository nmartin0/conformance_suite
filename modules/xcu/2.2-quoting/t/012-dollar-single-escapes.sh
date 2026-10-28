# Inside dollar-single-quotes the sequences backslash-double-quote,
# backslash-single-quote, double backslash, and backslash followed by a, b,
# e, f, n, r, t or v yield the double quote, single quote, backslash, alert,
# backspace, escape, form-feed, newline, carriage-return, tab and
# vertical-tab characters.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.2.2-quoting.012
. "$PC_CORE/pc.sh"
pc_begin XCU.2.2-quoting.012
PCSH=${PC_SH:-sh}
LC_ALL=C
export PCSH LC_ALL
PCW=$PWD
PCD=pcw$$
trap 'cd "$PCW" 2>/dev/null; rm -rf "$PCD"' 0
mkdir "$PCD" && cd "$PCD" || pc_fail "cannot make a working directory"
{
	printf 'printf \047%%s\\n\047 $\047\\a\\b\\e\\f\\n\\r\\t\\v\047 $\047\\"\\\047\\\\\047 $\047it\\\047s "q"\047\n'
} >s.sh || pc_fail "cannot write the test script"
pc_test
"$PCSH" ./s.sh </dev/null >out 2>err
rc=$?
want=$(printf '\007\010\033\014\n\015\t\013\n"\047\\\nit\047s "q"\n'; printf x)
got=$(cat out; printf x)
[ "$got" = "$want" ] || pc_fail "standard output was [$(cat out)]"
pc_pass
