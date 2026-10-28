# Fields, getline input, command-line values, split elements and ENVIRON values that look numeric compare as numbers, while string constants and concatenations compare as strings.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.awk.017
. "$PC_CORE/pc.sh"
pc_begin XCU.awk.017
LC_ALL=POSIX
export LC_ALL
NL='
'
pc_test
e() { awk "BEGIN { $1 }" 2>/dev/null </dev/null; }
[ "$(echo '10.0 10' | awk '{ print ($1 == $2) }' 2>/dev/null)" = "1" ] || pc_fail "two numeric-string fields were not compared numerically"
[ "$(e 'print ("10.0" == 10), ("10.0" == "10")')" = "0 0" ] || pc_fail "a string constant was compared as a number"
[ "$(echo abc | awk '{ print ($1 == 0) }' 2>/dev/null)" = "0" ] || pc_fail "a non-numeric field equalled 0"
[ "$(echo '1e2 100' | awk '{ print ($1 == $2) }' 2>/dev/null)" = "1" ] || pc_fail "1e2 and 100 were not equal as numeric strings"
[ "$(awk -v x=10.0 'BEGIN { print (x == 10) }' 2>/dev/null </dev/null)" = "1" ] || pc_fail "a -v value was not a numeric string"
[ "$(awk 'BEGIN { print (ARGV[1] == 5.0) }' 5 2>/dev/null </dev/null)" = "1" ] || pc_fail "an ARGV element was not a numeric string"
[ "$(echo 3.0 | awk 'BEGIN { getline v; print (v == 3) }' 2>/dev/null)" = "1" ] || pc_fail "a getline value was not a numeric string"
[ "$(N=7.0 awk 'BEGIN { print (ENVIRON["N"] == 7) }' 2>/dev/null </dev/null)" = "1" ] || pc_fail "an ENVIRON value was not a numeric string"
[ "$(echo 5.0 | awk '{ y = $1; print (y == 5) }' 2>/dev/null)" = "1" ] || pc_fail "a copy of a numeric string was not a numeric string"
[ "$(echo 5.0 | awk '{ y = $1 ""; print (y == 5) }' 2>/dev/null)" = "0" ] || pc_fail "a concatenation result was compared as a number"
[ "$(e 'x = "3.0"; print x + 0, x')" = "3 3.0" ] || pc_fail "converting a string to a number changed the string"
[ "$(e 'split("10.0 10", a); print (a[1] == a[2])')" = "1" ] || pc_fail "split elements were not numeric strings"
[ "$(echo '  12  ' | awk -F, '{ print ($1 == 12) }' 2>/dev/null)" = "1" ] || pc_fail "blanks around a numeric field prevented a numeric comparison"
pc_pass
