# In the POSIX locale the long format date is month, day and hour:minute for a recent file and month, day and year for an old or future one.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.ls.028
#
# The year of a future file rests on touch -t accepting a date in 2037,
# which the touch page requires.
. "$PC_CORE/pc.sh"
pc_begin XCU.ls.028
LC_ALL=POSIX; export LC_ALL
TZ=UTC0; export TZ
mkdir d || pc_fail "cannot create d"
touch -t 200101021504 d/old || pc_fail "cannot set time"
touch -t 203701021504 d/fut || pc_fail "cannot set time"
: >d/new || pc_fail "cannot create d/new"
pc_test
set -- $(ls -l d/old)
[ "$#" -eq 9 ] || pc_unresolved "unexpected number of fields ($#)"
[ "$6 $7 $8" = "Jan 2 2001" ] || pc_fail "old file date is '$6 $7 $8'"
set -- $(ls -l d/fut)
[ "$#" -eq 9 ] || pc_unresolved "unexpected number of fields ($#)"
[ "$6 $7 $8" = "Jan 2 2037" ] || pc_fail "future file date is '$6 $7 $8'"
set -- $(ls -l d/new)
[ "$#" -eq 9 ] || pc_unresolved "unexpected number of fields ($#)"
case $8 in
[0-9][0-9]:[0-9][0-9]) ;;
*) pc_fail "recent file time is '$8'" ;;
esac
pc_pass
