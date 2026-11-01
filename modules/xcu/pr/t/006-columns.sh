# -column writes down columns, -a fills across.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.pr.006
# MUTANT    pr.c_ignored
# MUTANT    pr.c_across
# MUTANT    pr.a_ignored
# MUTANT    pr.c_plus1
# MUTANT    pr.c_extra_line
# MUTANT    pr.c_swapped
# MUTANT    pr.c_multidigit
# VARIANT   pr.pad_to_width
. "$PC_CORE/pc.sh"
pc_begin XCU.pr.006
LC_ALL=POSIX
export LC_ALL
printf 'a\nb\nc\nd\n' >f || pc_unresolved "cannot create input"
printf 'a\nb\nc\nd\ne\nf\n' >f6 || pc_unresolved "cannot create input"
printf 'a\nb\nc\nd\ne\nf\ng\nh\ni\nj\n' >f10 || pc_unresolved "cannot create input"
row() { printf '%s\n' "$1" | sed -n "$2p"; }
pc_test
got=$(pr -t -2 f 2>/dev/null </dev/null)
set -- $(row "$got" 1)
[ "$#" -eq 2 ] && [ "$1" = a ] && [ "$2" = c ] || pc_fail "-2 first row is not a and c"
set -- $(row "$got" 2)
[ "$#" -eq 2 ] && [ "$1" = b ] && [ "$2" = d ] || pc_fail "-2 second row is not b and d"
n=$(pr -t -2 f 2>/dev/null </dev/null | wc -l)
[ "$n" -eq 2 ] || pc_fail "-2 -t did not use the minimum number of lines"
got=$(pr -t -2 -a f 2>/dev/null </dev/null)
set -- $(row "$got" 1)
[ "$#" -eq 2 ] && [ "$1" = a ] && [ "$2" = b ] || pc_fail "-a first row is not a and b"
set -- $(row "$got" 2)
[ "$#" -eq 2 ] && [ "$1" = c ] && [ "$2" = d ] || pc_fail "-a second row is not c and d"
n=$(pr -t -2 -a f 2>/dev/null </dev/null | wc -l)
[ "$n" -eq 2 ] || pc_fail "-2 -a -t did not use the minimum number of lines"
got=$(pr -t -3 f6 2>/dev/null </dev/null)
set -- $(row "$got" 1)
[ "$#" -eq 3 ] && [ "$1" = a ] && [ "$2" = c ] && [ "$3" = e ] || pc_fail "-3 first row is not a, c and e"
set -- $(row "$got" 2)
[ "$#" -eq 3 ] && [ "$1" = b ] && [ "$2" = d ] && [ "$3" = f ] || pc_fail "-3 second row is not b, d and f"
n=$(pr -t -3 f6 2>/dev/null </dev/null | wc -l)
[ "$n" -eq 2 ] || pc_fail "-3 -t did not use the minimum number of lines"
got=$(pr -t -10 f10 2>/dev/null </dev/null)
set -- $got
[ "$#" -eq 10 ] && [ "$1" = a ] && [ "$5" = e ] && [ "${10}" = j ] || pc_fail "-10 did not write ten columns"
n=$(pr -t -10 f10 2>/dev/null </dev/null | wc -l)
[ "$n" -eq 1 ] || pc_fail "-10 -t of ten lines did not use one line"
pc_pass
