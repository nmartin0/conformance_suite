# eval arithmetic, radix and width
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.m4.006
# eval evaluates the listed C operators with C precedence, accepts octal and
# hexadecimal constants, and takes a radix and a minimum digit count as
# second and third arguments.
. "$PC_CORE/pc.sh"
pc_begin XCU.m4.006
LC_ALL=C; export LC_ALL
pc_top=$(pwd) || pc_unresolved "cannot find the working directory"
pc_wd=pcw.$$
mkdir "$pc_wd" && cd "$pc_wd" || pc_unresolved "cannot make a work directory"
trap 'cd "$pc_top" && rm -rf "$pc_wd"' EXIT
cat >in <<'EOM'
eval(2+3*4)|eval(10/3)|eval(10%3)|eval(-7+2)|eval(1<<4)|eval(7>>1)|eval(3==3)|eval(3!=3)|eval(1&&0)|eval(1||0)|eval(6&3)|eval(6|3)|eval(6^3)|eval(~0)|eval(!5)|eval((2+3)*4)|eval(0x1f)|eval(010)|eval(255,16)|eval(5,2,8)|eval(3<4)|eval(4<=3)|eval(5>=5)|eval(5>4)|eval(7-2-1)
EOM
printf '14|3|1|-5|16|3|1|0|0|1|2|7|5|-1|0|20|31|8|ff|00000101|1|0|1|1|4\n' >exp
pc_test
m4 in >out 2>err </dev/null || pc_fail "m4: nonzero status"
cmp out exp >/dev/null 2>&1 || pc_fail "output differs: $(cat out)"
pc_pass
