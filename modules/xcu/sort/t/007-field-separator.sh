# sort -t char -k n orders by the nth field, counting every occurrence of char as a separator.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.sort.007
. "$PC_CORE/pc.sh"
pc_begin XCU.sort.007
LC_ALL=POSIX
export LC_ALL
NL='
'
printf 'x:3:b\ny:1:c\nz:2:a\n' >in1 || pc_fail "cannot create input"
printf 'm::1\nn:b:2\no:a:3\n' >in2 || pc_fail "cannot create input"
pc_test
got=$(sort -t : -k 2 in1 2>/dev/null </dev/null; printf 'rc=%s' "$?")
[ "$got" = "y:1:c${NL}z:2:a${NL}x:3:b${NL}rc=0" ] || pc_fail "-t : -k 2 order is wrong"
got=$(sort -t : -k 3,3 in1 2>/dev/null </dev/null; printf 'rc=%s' "$?")
[ "$got" = "z:2:a${NL}x:3:b${NL}y:1:c${NL}rc=0" ] || pc_fail "-t : -k 3,3 order is wrong"
got=$(sort -t : -k 2,2 in2 2>/dev/null </dev/null; printf 'rc=%s' "$?")
[ "$got" = "m::1${NL}o:a:3${NL}n:b:2${NL}rc=0" ] || pc_fail "an empty field did not sort first"
pc_pass
