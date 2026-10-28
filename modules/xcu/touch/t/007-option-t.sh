# touch -t sets both times to the given time, interpreting it in the TZ time zone, in the forms CCYYMMDDhhmm, YYMMDDhhmm and with .SS seconds.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.touch.007
. "$PC_CORE/pc.sh"
pc_begin XCU.touch.007
TZ=UTC0; export TZ
: >f1 || pc_fail "cannot create f1"
: >f2 || pc_fail "cannot create f2"
: >f3 || pc_fail "cannot create f3"
: >f4 || pc_fail "cannot create f4"
: >f5 || pc_fail "cannot create f5"
pc_test
touch -t 200102030405 f1 || pc_fail "touch -t with a four digit year failed"
set -- $(ls -l f1)
[ "$6 $7 $8" = "Feb 3 2001" ] || pc_fail "CCYYMMDDhhmm gave $6 $7 $8"
set -- $(ls -lu f1)
[ "$6 $7 $8" = "Feb 3 2001" ] || pc_fail "access time is $6 $7 $8"
touch -t 0102030405 f2 || pc_fail "touch -t with a two digit year failed"
set -- $(ls -l f2)
[ "$6 $7 $8" = "Feb 3 2001" ] || pc_fail "YYMMDDhhmm (01) gave $6 $7 $8"
touch -t 9902030405 f3 || pc_fail "touch -t with year 99 failed"
set -- $(ls -l f3)
[ "$6 $7 $8" = "Feb 3 1999" ] || pc_fail "YYMMDDhhmm (99) gave $6 $7 $8"
touch -t 200102030405.00 f4 || pc_fail "touch -t with seconds failed"
touch -t 200102030405.30 f5 || pc_fail "touch -t with seconds failed"
[ f5 -nt f4 ] || pc_fail "the seconds were ignored"
touch -t 200102030406 f2 || pc_fail "touch -t failed"
[ f2 -nt f1 ] || pc_fail "the minutes were ignored"
pc_pass
