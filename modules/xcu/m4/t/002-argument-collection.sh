# argument collection, nesting, $* and $@
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.m4.002
# Parenthesised commas do not split arguments, leading white space of an
# argument is dropped and trailing kept, omitted arguments are null, $*
# joins with commas, and $@ quotes each argument so it is not expanded
# again.
# MUTANT    m4.paren_split
# MUTANT    m4.lead_ws_kept
# MUTANT    m4.lead_ws_space_only
# MUTANT    m4.trail_ws_dropped
# MUTANT    m4.star_space
# MUTANT    m4.star_quoted
# MUTANT    m4.star_first_only
# MUTANT    m4.at_unquoted
# MUTANT    m4.at_space
# MUTANT    m4.args_not_expanded
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
f(`a,b',c)
f(	a,
b)
define(`n',`$#')define(`cm',`x,y')dnl
n(cm,z) f(cm,z)
n((a,b)) n(`a,b')
EOM
printf '[(a,b)][c]\n[a][]\n[a ][b ]\n[a][b]\nM,M|m,m\n[a,b][c]\n[a][b]\n3 [x][y]\n1 1\n' >exp
pc_test
m4 in >out 2>err </dev/null || pc_fail "m4: nonzero status"
cmp out exp >/dev/null 2>&1 || pc_fail "output differs: $(cat out)"
pc_pass
