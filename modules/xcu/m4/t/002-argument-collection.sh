# argument collection, nesting, $* and $@
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.m4.002
# Parenthesised commas do not split arguments, leading white space of an
# argument is dropped and trailing kept, omitted arguments are null, $*
# joins with commas, and $@ quotes each argument so it is not expanded
# again.
. "$PC_CORE/pc.sh"
pc_begin XCU.m4.002
LC_ALL=C; export LC_ALL
pc_top=$(pwd) || pc_unresolved "cannot find the working directory"
pc_wd=pcw.$$
mkdir "$pc_wd" && cd "$pc_wd" || pc_unresolved "cannot make a work directory"
trap 'cd "$pc_top" && rm -rf "$pc_wd"' EXIT
cat >in <<'EOM'
define(`m',`M')define(`f',`[$1][$2]')define(`g',`$*|$@')dnl
f((a,b),c)
f(a)
f( a , b )
f(a,b,c,d)
g(`m',`m')
EOM
printf '[(a,b)][c]\n[a][]\n[a ][b ]\n[a][b]\nM,M|m,m\n' >exp
pc_test
m4 in >out 2>err </dev/null || pc_fail "m4: nonzero status"
cmp out exp >/dev/null 2>&1 || pc_fail "output differs: $(cat out)"
pc_pass
