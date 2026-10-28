# The scale register and the scale rules of the operators, sqrt, length and scale() decide how many fractional digits are kept, truncating the rest.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.bc.002
. "$PC_CORE/pc.sh"
pc_begin XCU.bc.002
LC_ALL=POSIX
export LC_ALL
NL='
'
pc_test
b() { printf '%b\n' "$1" | bc 2>/dev/null; }
z() { r=$1; case $r in 0.*) r=${r#0} ;; -0.*) r=-${r#-0} ;; esac; printf '%s' "$r"; }
[ "$(b '1/3')" = "0" ] || pc_fail "1/3 at scale 0 not 0"
[ "$(b '7/2')" = "3" ] || pc_fail "7/2 at scale 0 not 3"
[ "$(b '3.2/1')" = "3" ] || pc_fail "3.2/1 at scale 0 not truncated to 3"
[ "$(z "$(b 'scale=5\n1/3')")" = ".33333" ] || pc_fail "1/3 at scale 5 wrong"
[ "$(b 'scale=2\n10/4')" = "2.50" ] || pc_fail "10/4 at scale 2 not 2.50"
[ "$(z "$(b 'scale=3\n2/3')")" = ".666" ] || pc_fail "2/3 at scale 3 was not truncated to .666"
[ "$(b 'scale')" = "0" ] || pc_fail "the default scale was not 0"
[ "$(b 'scale=20\nscale')" = "20" ] || pc_fail "assigning scale wrong"
[ "$(b 'scale=2.9\nscale')" = "2" ] || pc_fail "a fractional value assigned to scale was not truncated"
[ "$(b 'length(123.45)')" = "5" ] || pc_fail "length(123.45) wrong"
[ "$(b 'length(12345)')" = "5" ] || pc_fail "length(12345) wrong"
[ "$(b 'scale(123.45)')" = "2" ] || pc_fail "scale(123.45) wrong"
[ "$(b 'scale(7)')" = "0" ] || pc_fail "scale(7) wrong"
[ "$(b 'scale(1.500)')" = "3" ] || pc_fail "scale(1.500) wrong"
[ "$(b 'sqrt(16)')" = "4" ] || pc_fail "sqrt(16) wrong"
[ "$(b 'scale=5\nsqrt(2)')" = "1.41421" ] || pc_fail "sqrt(2) at scale 5 not truncated to 1.41421"
[ "$(b 'scale=2\nsqrt(16)')" = "4.00" ] || pc_fail "the scale of sqrt(16) at scale 2 wrong"
[ "$(b '1.50+0')" = "1.50" ] || pc_fail "the scale of a sum wrong"
[ "$(b '.5+.5')" = "1.0" ] || pc_fail "the scale of .5+.5 wrong"
[ "$(b 'scale=2\n1+1')" = "2" ] || pc_fail "the scale of 1+1 was taken from the scale register"
[ "$(b '1.5*1.5')" = "2.2" ] || pc_fail "the scale of 1.5*1.5 at scale 0 wrong"
[ "$(b 'scale=2\n1.5*1.5')" = "2.25" ] || pc_fail "1.5*1.5 at scale 2 wrong"
[ "$(z "$(b 'scale=3\n10%3')")" = ".001" ] || pc_fail "10%3 at scale 3 wrong"
[ "$(b '7.25-0.25')" = "7.00" ] || pc_fail "the scale of a difference wrong"
[ "$(b 'scale=3\n-1/8')" = "-.125" ] || [ "$(b 'scale=3\n-1/8')" = "-0.125" ] || pc_fail "a negative fraction wrong"
pc_pass
