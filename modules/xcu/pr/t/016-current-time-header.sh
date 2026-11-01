# The header date is the current time for -m and for standard input.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.pr.016
# MUTANT    pr.merge_date_fixed
# MUTANT    pr.stdin_date_fixed
. "$PC_CORE/pc.sh"
pc_begin XCU.pr.016
LC_ALL=POSIX
export LC_ALL
printf 'a1\na2\n' >f1 || pc_unresolved "cannot create input"
printf 'b1\nb2\n' >f2 || pc_unresolved "cannot create input"
TZ=UTC0 touch -t 202001020304 f1 f2 || pc_unresolved "cannot set the modification time"
pc_test
d1=$(TZ=UTC0 date "+%b %e %H:%M %Y")
got=$(TZ=UTC0 pr -m -l 15 f1 f2 2>/dev/null </dev/null)
d2=$(TZ=UTC0 date "+%b %e %H:%M %Y")
l=$(printf '%s\n' "$got" | sed -n 3p)
[ "$l" = "$d1  Page 1" ] || [ "$l" = "$d2  Page 1" ] || pc_fail "the date of the -m header is not the current time"
d1=$(TZ=UTC0 date "+%b %e %H:%M %Y")
got=$(TZ=UTC0 pr -l 15 <f1 2>/dev/null)
d2=$(TZ=UTC0 date "+%b %e %H:%M %Y")
l=$(printf '%s\n' "$got" | sed -n 3p)
[ "$l" = "$d1  Page 1" ] || [ "$l" = "$d2  Page 1" ] || pc_fail "the date of a page from standard input is not the current time"
pc_pass
