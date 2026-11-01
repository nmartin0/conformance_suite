# define, arguments and rescanning
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.m4.001
# A macro defined with define expands with $1, $2, $0 and $# replaced;
# without parentheses $# is 0 and arguments are null; the expansion is
# rescanned; names are the longest run of letters, digits and underscores
# not starting with a digit.
# MUTANT    m4.ok_status1
# MUTANT    m4.final_nl_dropped
# MUTANT    m4.final_nl_added
# MUTANT    m4.arg_off_by_one
# MUTANT    m4.arg_swap12
# MUTANT    m4.dollar0_empty
# MUTANT    m4.hash_noparen_1
# MUTANT    m4.hash_plus1
# MUTANT    m4.hash_empty_zero
# MUTANT    m4.hash_cap9
# MUTANT    m4.max_args_8
# MUTANT    m4.no_rescan
# MUTANT    m4.name_digit_start
# MUTANT    m4.name_no_underscore
# MUTANT    m4.name_no_digit
# MUTANT    m4.name_shortest
# VARIANT   m4.engine
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
define(`x',X)1x 2x_x x2
define(`_u',U)_u _u1
define(`n',`$#')n n() n(,) n(a,b,c)
define(`nine',`$9|$#|$1')nine(1,2,3,4,5,6,7,8,9,10,11,12)
EOM
printf '<a|b|2|f><||0|f>\nc1\nabc ab_ ab1 X\n1X 2x_x x2\nU _u1\n0 1 2 3\n9|12|1\n' >exp
pc_test
m4 in >out 2>err </dev/null || pc_fail "m4: nonzero status"
cmp out exp >/dev/null 2>&1 || pc_fail "output differs: $(cat out)"
pc_pass
