# The first line of ipcs.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.ipcs.001
. "$PC_CORE/pc.sh"
pc_begin XCU.ipcs.001
LC_ALL=C; export LC_ALL
TZ=UTC0; export TZ
pc_test
ipcs >out 2>err </dev/null
rc=$?
[ "$rc" -eq 0 ] || pc_fail "ipcs exited with status $rc"
read -r first <out
case $first in
"IPC status from "*" as of "*) ;;
*) pc_fail "the first line '$first' is not of the form 'IPC status from SOURCE as of DATE'" ;;
esac
dt=${first##* as of }
set -- $dt
[ "$#" -eq 6 ] || pc_fail "the date '$dt' does not have six words"
case $1 in Sun | Mon | Tue | Wed | Thu | Fri | Sat) ;; *) pc_fail "weekday '$1' in the date" ;; esac
case $2 in Jan | Feb | Mar | Apr | May | Jun | Jul | Aug | Sep | Oct | Nov | Dec) ;; *) pc_fail "month '$2' in the date" ;; esac
case $3 in [1-9] | [12][0-9] | 3[01]) ;; *) pc_fail "day '$3' in the date" ;; esac
case $4 in [0-2][0-9]:[0-5][0-9]:[0-6][0-9]) ;; *) pc_fail "time '$4' in the date" ;; esac
case $6 in [0-9][0-9][0-9][0-9]) ;; *) pc_fail "year '$6' in the date" ;; esac
pc_pass
