# awk expression operators follow the precedence, associativity, conversion and comparison rules of the page.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.awk.008
# MUTANT    awk.pow_as_mul
# MUTANT    awk.pow_left_assoc
# MUTANT    awk.unary_minus_binds_tight
# MUTANT    awk.logical_not_ignored
# MUTANT    awk.post_inc_returns_new
# MUTANT    awk.pre_inc_returns_old
# MUTANT    awk.post_dec_returns_new
# MUTANT    awk.pre_dec_returns_old
# MUTANT    awk.post_inc_noop
# MUTANT    awk.add_assign_as_assign
# MUTANT    awk.sub_assign_as_add
# MUTANT    awk.mul_assign_as_add
# MUTANT    awk.div_assign_as_mul
# MUTANT    awk.mod_assign_as_div
# MUTANT    awk.pow_assign_as_mul
# MUTANT    awk.cond_arms_swapped
# MUTANT    awk.and_or_swapped
# MUTANT    awk.no_short_circuit
# MUTANT    awk.const_digits_numeric
# MUTANT    awk.fields_always_strings
# MUTANT    awk.match_inverted
# MUTANT    awk.in_creates_element
. "$PC_CORE/pc.sh"
pc_begin XCU.awk.008
LC_ALL=POSIX
export LC_ALL
NL='
'
pc_test
e() { awk "BEGIN { $1 }" 2>/dev/null </dev/null; }
[ "$(e 'print 2+3*4, (2+3)*4, 2^3^2, -2^2, 7%3, 7/2')" = "14 20 512 -4 1 3.5" ] || pc_fail "arithmetic or precedence wrong"
[ "$(e 'print -7%3, 1/4')" = "-1 0.25" ] || pc_fail "modulus of a negative or division wrong"
[ "$(e 'print 1+2 " " 3, 1 " " 2+3')" = "3 3 1 5" ] || pc_fail "concatenation precedence wrong"
[ "$(e 'print !0, !1, !"", !"a"')" = "1 0 1 0" ] || pc_fail "logical not wrong"
[ "$(e 'x=5; a=x++; b=x; c=++x; d=x--; f=--x; print a, b, c, d, f, x')" = "5 6 7 7 5 5" ] || pc_fail "increment or decrement wrong"
[ "$(e 'x=11; x+=5; x-=3; x*=2; x/=4; x%=4; x^=2; print x')" = "6.25" ] || pc_fail "assignment operators wrong"
[ "$(e 'print (1?"a":"b"), (0?"a":"b"), (""?"t":"f")')" = "a b f" ] || pc_fail "conditional expression wrong"
[ "$(e 'print (1&&0), (1||0), (0||0), ("a"&&"b")')" = "0 1 0 1" ] || pc_fail "&& or || wrong"
[ "$(e 'x=0; 0 && x++; 1 || x++; print x')" = "0" ] || pc_fail "&& or || did not short-circuit"
[ "$(e 'print ("10"<"9"), (10<9)')" = "1 0" ] || pc_fail "string and numeric comparison wrong"
[ "$(e 'print (x==0), (x==""), length(x)')" = "1 1 0" ] || pc_fail "an uninitialized variable was not both 0 and empty"
[ "$(e 'print ("abc" ~ /b/), ("abc" ~ "^b"), ("abc" !~ /z/), ("a.c" ~ "a\\.c")')" = "1 0 1 1" ] || pc_fail "~ or !~ wrong"
[ "$(e 'a["x"]; print ("x" in a), ("y" in a), length(a)')" = "1 0 1" ] || pc_fail "in operator wrong"
[ "$(e 'x=1e6; print x ""; y=0.1+0.2; print y ""')" = "1000000${NL}0.3" ] || pc_fail "number to string conversion wrong"
[ "$(e 'print 1e6, 30000*30000')" = "1000000 900000000" ] || pc_fail "integral values printed wrongly"
[ "$(e 'print "3x"+1, "x"+1, " 12 "+0, "1e2"+0, ".5"+0')" = "4 1 12 100 0.5" ] || pc_fail "string to number conversion wrong"
[ "$(e 'x = "A"; y = x x; print y, (y == "AA")')" = "AA 1" ] || pc_fail "concatenation of variables wrong"
[ "$(e 'x = (1 < 2) + (2 < 1); print x, 3 - -2')" = "1 5" ] || pc_fail "comparison results are not 1 and 0"
got=$(echo "10 9" | awk '{ print ($1 > $2), ($1 < $2), ($1 == 10) }' 2>/dev/null)
[ "$got" = "1 0 1" ] || pc_fail "fields that look numeric were not compared numerically"
got=$(echo "abc 10" | awk '{ print ($1 > $2) }' 2>/dev/null)
[ "$got" = "1" ] || pc_fail "a non-numeric field was not compared as a string"
pc_pass
