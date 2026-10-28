# define, arguments and rescanning
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.m4.001
# A macro defined with define expands with $1, $2, $0 and $# replaced;
# without parentheses $# is 0 and arguments are null; the expansion is
# rescanned; names are the longest run of letters, digits and underscores
# not starting with a digit.
. "$PC_CORE/pc.sh"
pc_begin XCU.m4.001
LC_ALL=C; export LC_ALL
pc_top=$(pwd) || pc_unresolved "cannot find the working directory"
pc_wd=pcw.$$
mkdir "$pc_wd" && cd "$pc_wd" || pc_unresolved "cannot make a work directory"
trap 'cd "$pc_top" && rm -rf "$pc_wd"' EXIT
cat >in <<'EOM'
define(`f',`<$1|$2|$#|`$0'>')dnl
f(a, b)f
define(`a1',`b1')define(`b1',`c1')a1
define(`ab',X)abc ab_ ab1 ab
EOM
printf '<a|b|2|f><||0|f>\nc1\nabc ab_ ab1 X\n' >exp
pc_test
m4 in >out 2>err </dev/null || pc_fail "m4: nonzero status"
cmp out exp >/dev/null 2>&1 || pc_fail "output differs: $(cat out)"
pc_pass
