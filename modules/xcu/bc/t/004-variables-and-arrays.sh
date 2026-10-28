# Variables and arrays hold values, start at zero, keep separate namespaces, truncate subscripts, and support ++, -- and compound assignment.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.bc.004
. "$PC_CORE/pc.sh"
pc_begin XCU.bc.004
LC_ALL=POSIX
export LC_ALL
NL='
'
pc_test
b() { printf '%b\n' "$1" | bc 2>/dev/null; }
[ "$(b 'z')" = "0" ] || pc_fail "an unset variable was not 0"
[ "$(b 'a[5]')" = "0" ] || pc_fail "an unset array element was not 0"
[ "$(b 'a[0]=1\na[1]=2\na[0]+a[1]')" = "3" ] || pc_fail "array elements wrong"
[ "$(b 'a[1.9]=7\na[1]')" = "7" ] || pc_fail "a fractional subscript was not truncated"
[ "$(b 'a=4\na[0]=9\na\na[0]')" = "4${NL}9" ] || pc_fail "a variable and an array of one name interfered"
[ "$(b 'x=5\nx++\nx')" = "5${NL}6" ] || pc_fail "x++ wrong"
[ "$(b 'x=5\n++x\nx')" = "6${NL}6" ] || pc_fail "++x wrong"
[ "$(b 'x=5\nx--\nx')" = "5${NL}4" ] || pc_fail "x-- wrong"
[ "$(b 'x=5\n--x\nx')" = "4${NL}4" ] || pc_fail "--x wrong"
[ "$(b 'x=10\nx+=5\nx')" = "15" ] || pc_fail "+= wrong"
[ "$(b 'x=10\nx-=3\nx')" = "7" ] || pc_fail "-= wrong"
[ "$(b 'x=10\nx*=2\nx')" = "20" ] || pc_fail "*= wrong"
[ "$(b 'x=12\nx/=4\nx')" = "3" ] || pc_fail "/= wrong"
[ "$(b 'x=14\nx%=4\nx')" = "2" ] || pc_fail "%= wrong"
[ "$(b 'x=2\nx^=3\nx')" = "8" ] || pc_fail "^= wrong"
[ "$(b 'a[0]=1\na[0]+=5\na[0]')" = "6" ] || pc_fail "compound assignment on an array element wrong"
pc_pass
