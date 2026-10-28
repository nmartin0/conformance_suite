# words, names and built-in function tokens
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.awk.024
# A word is made of underscores, digits and letters, so names that begin
# with a keyword or built-in function name are ordinary names; a built-in
# function name may be followed by a blank before its parenthesis, and a
# user function is called with the parenthesis right after the name.
. "$PC_CORE/pc.sh"
pc_begin XCU.awk.024
LC_ALL=POSIX
export LC_ALL
NL='
'
pc_test
got=$(awk 'BEGIN { ifx = 5; whilex = 6; indexer = 7; getlinex = 8; BEGINx = 9; print ifx, whilex, indexer, getlinex, BEGINx; _a_1 = 2; print _a_1 }' 2>/dev/null </dev/null)
[ "$got" = "5 6 7 8 9${NL}2" ] || pc_fail "names starting with keywords: $got"
got=$(awk 'BEGIN { print substr ("hello", 2, 3), index ("abc", "c"), length ("xyz"), toupper ("a") }' 2>/dev/null </dev/null)
[ "$got" = "ell 3 3 A" ] || pc_fail "a built-in function with a blank before the parenthesis: $got"
got=$(awk 'function dbl(x) { return x * 2 } BEGIN { print dbl(3) }' 2>/dev/null </dev/null)
[ "$got" = "6" ] || pc_fail "a user function call: $got"
got=$(awk 'BEGIN { a_b = 1; a = 2; print a_b, a }' 2>/dev/null </dev/null)
[ "$got" = "1 2" ] || pc_fail "an underscore inside a name: $got"
pc_pass
