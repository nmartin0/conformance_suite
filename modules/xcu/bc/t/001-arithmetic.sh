# bc evaluates integer expressions with arbitrary precision using the stated operators, precedence and associativity, writing the value of each expression that is not an assignment.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.bc.001
. "$PC_CORE/pc.sh"
pc_begin XCU.bc.001
LC_ALL=POSIX
export LC_ALL
NL='
'
pc_test
b() { printf '%b\n' "$1" | bc 2>/dev/null; }
[ "$(b '1+2')" = "3" ] || pc_fail "1+2 wrong"
[ "$(b '2*3+4')" = "10" ] || pc_fail "2*3+4 wrong"
[ "$(b '2+3*4^2')" = "50" ] || pc_fail "precedence of ^ over * over + wrong"
[ "$(b '2^3^2')" = "512" ] || pc_fail "^ did not bind right to left"
[ "$(b '7-10')" = "-3" ] || pc_fail "a negative result wrong"
[ "$(b '(2+3)*4')" = "20" ] || pc_fail "parentheses wrong"
[ "$(b '10-3-2')" = "5" ] || pc_fail "- did not bind left to right"
[ "$(b '100/5/2')" = "10" ] || pc_fail "/ did not bind left to right"
[ "$(b '10%3')" = "1" ] || pc_fail "10%3 wrong"
[ "$(b '-7%3')" = "-1" ] || pc_fail "-7%3 wrong"
[ "$(b '2^10')" = "1024" ] || pc_fail "2^10 wrong"
[ "$(b '100/7')" = "14" ] || pc_fail "integer division wrong"
[ "$(b '2^100')" = "1267650600228229401496703205376" ] || pc_fail "2^100 not computed exactly"
[ "$(b '10^30+1')" = "1000000000000000000000000000001" ] || pc_fail "10^30+1 not computed exactly"
[ "$(b '0')" = "0" ] || pc_fail "zero wrong"
[ "$(b '1-1')" = "0" ] || pc_fail "1-1 wrong"
[ "$(b '  1  +  2 ')" = "3" ] || pc_fail "blanks changed the result"
[ "$(b '1;2')" = "1${NL}2" ] || pc_fail "semicolon-separated expressions were not both written"
[ "$(b '3\n4')" = "3${NL}4" ] || pc_fail "newline-separated expressions were not both written"
got=$(b 'x=5')
[ -z "$got" ] || pc_fail "an assignment statement wrote output"
[ "$(b 'x=5\nx')" = "5" ] || pc_fail "a stored value wrong"
[ "$(b 'x=5\nx+1')" = "6" ] || pc_fail "using a stored value wrong"
[ "$(b 'a=b=4\na+b')" = "8" ] || pc_fail "chained assignment wrong"
[ "$(b '123456789*987654321')" = "121932631112635269" ] || pc_fail "a large product wrong"
printf '1+1\n' | bc >/dev/null 2>&1
[ $? -eq 0 ] || pc_fail "status was not 0 for a valid program"
pc_pass
