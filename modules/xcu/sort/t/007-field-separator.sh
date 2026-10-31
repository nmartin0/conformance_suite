# sort -t char -k n orders by the nth field, counting every occurrence of char as a separator.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.sort.007
# MUTANT    sort.t_ignored
# MUTANT    sort.t_collapse
# MUTANT    sort.t_sep_in_field
# MUTANT    sort.open_to_field
# MUTANT    sort.k_field_off1
. "$PC_CORE/pc.sh"
pc_begin XCU.sort.007
LC_ALL=POSIX
export LC_ALL
NL='
'
printf 'x:3:b\ny:1:c\nz:2:a\n' >in1 || pc_fail "cannot create input"
printf 'm::z\nn:b:2\no:a:3\n' >in2 || pc_fail "cannot create input"
printf 'x:1:b\ny:1:a\n' >in3 || pc_fail "cannot create input"
printf 'a:10\nb:9\nc:100\n' >in4 || pc_fail "cannot create input"
pc_test
got=$(sort -t : -k 2 in1 2>/dev/null </dev/null; printf 'rc=%s' "$?")
[ "$got" = "y:1:c${NL}z:2:a${NL}x:3:b${NL}rc=0" ] || pc_fail "-t : -k 2 order is wrong"
got=$(sort -t : -k 3,3 in1 2>/dev/null </dev/null; printf 'rc=%s' "$?")
[ "$got" = "z:2:a${NL}x:3:b${NL}y:1:c${NL}rc=0" ] || pc_fail "-t : -k 3,3 order is wrong"
got=$(sort -t : -k 2,2 in2 2>/dev/null </dev/null; printf 'rc=%s' "$?")
[ "$got" = "m::z${NL}o:a:3${NL}n:b:2${NL}rc=0" ] || pc_fail "an empty field did not sort first"
got=$(sort -t : -k 2 in3 2>/dev/null </dev/null; printf 'rc=%s' "$?")
[ "$got" = "y:1:a${NL}x:1:b${NL}rc=0" ] || pc_fail "-k 2 did not run to the end of the line"
got=$(sort -t : -k 2,2n in4 2>/dev/null </dev/null; printf 'rc=%s' "$?")
[ "$got" = "b:9${NL}a:10${NL}c:100${NL}rc=0" ] || pc_fail "the separator was taken as part of the numeric field"
pc_pass
