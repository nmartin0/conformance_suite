# eval arithmetic, radix and width
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.m4.006
# eval evaluates the listed C operators with C precedence, accepts octal and
# hexadecimal constants, and takes a radix and a minimum digit count as
# second and third arguments.
# MUTANT    m4.eval_flat_muldiv_add
# MUTANT    m4.eval_shift_above_add
# MUTANT    m4.eval_eq_with_rel
# MUTANT    m4.eval_or_with_xor
# MUTANT    m4.eval_xor_above_and
# MUTANT    m4.eval_andand_with_oror
# MUTANT    m4.eval_and_above_eq
# MUTANT    m4.eval_and_with_or
# MUTANT    m4.eval_right_assoc
# MUTANT    m4.eval_bang_low
# MUTANT    m4.eval_tilde_low
# MUTANT    m4.eval_tilde_neg
# MUTANT    m4.eval_not_bitwise
# MUTANT    m4.eval_not_neg
# MUTANT    m4.eval_unary_plus_missing
# MUTANT    m4.eval_floor_div
# MUTANT    m4.eval_floor_mod
# MUTANT    m4.eval_logical_value
# MUTANT    m4.eval_le_as_lt
# MUTANT    m4.eval_ge_as_gt
# MUTANT    m4.eval_ne_as_eq
# MUTANT    m4.eval_wrap16
# MUTANT    m4.eval_octal_decimal
# MUTANT    m4.eval_hex_missing
# MUTANT    m4.eval_hex_upperx
# MUTANT    m4.eval_radix_ignored
# MUTANT    m4.eval_width_ignored
# MUTANT    m4.eval_width_trunc
# MUTANT    m4.eval_width_space
# MUTANT    m4.eval_blank_radix_error
# VARIANT   m4.engine
# VARIANT   m4.eval_upper
. "$PC_CORE/pc.sh"
pc_begin XCU.m4.006
LC_ALL=C; export LC_ALL
pc_top=$(pwd) || pc_unresolved "cannot find the working directory"
pc_wd=pcw.$$
mkdir "$pc_wd" && cd "$pc_wd" || pc_unresolved "cannot make a work directory"
trap 'cd "$pc_top" && rm -rf "$pc_wd"' EXIT
cat >in <<'EOM'
eval(2+3*4)|eval(10/3)|eval(10%3)|eval(-7+2)|eval(1<<4)|eval(7>>1)|eval(3==3)|eval(3!=3)|eval(1&&0)|eval(1||0)|eval(6&3)|eval(6|3)|eval(6^3)|eval(~0)|eval(!5)|eval((2+3)*4)|eval(0x1f)|eval(010)|eval(255,16)|eval(5,2,8)|eval(3<4)|eval(4<=3)|eval(5>=5)|eval(5>4)|eval(7-2-1)
eval(1+2<<1)|eval(3==3<2)|eval(1<2==1)|eval(~0==-1)|eval(!5==1)|eval(1|2^3)|eval(1^3&2)|eval(1||0&&0)|eval(6&3|1)|eval(1&3==1)|eval(1|2&0)|eval(100/10/5)|eval(2*3%4)|eval(-7/2)|eval(-7%3)|eval(+3)|eval(-(2+3))|eval(-2*-3)|eval(5&&7)|eval(0||9)|eval(!0)|eval(3<=3)|eval(3!=4)|eval(2>3)
eval(0XfF)|eval(017)|eval(0)|eval(40000+30000)|eval(2147483646+1)|eval(8,8)|eval(5,2)|eval(35,36)|eval(255,10,2)|eval(5,10,3)|eval(10,,4)
EOM
printf '14|3|1|-5|16|3|1|0|0|1|2|7|5|-1|0|20|31|8|ff|00000101|1|0|1|1|4\n6|0|1|1|0|1|3|1|3|0|1|2|2|-3|-1|3|-5|6|1|1|1|1|1|0\n255|15|0|70000|2147483647|10|101|z|255|005|0010\n' >exp
pc_test
m4 in >out 2>err </dev/null || pc_fail "m4: nonzero status"
# the case of digits above 9 in a radix result is not stated: fold it
tr '[:upper:]' '[:lower:]' <out >outl
cmp outl exp >/dev/null 2>&1 || pc_fail "output differs: $(cat out)"
pc_pass
