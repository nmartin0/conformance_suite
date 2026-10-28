# touch -d accepts YYYY-MM-DDThh:mm:SS with an optional fraction and Z, takes T or a space, and reads a time without Z in the TZ time zone.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.touch.008
. "$PC_CORE/pc.sh"
pc_begin XCU.touch.008
TZ=UTC0; export TZ
: >f1 || pc_fail "cannot create f1"
: >f2 || pc_fail "cannot create f2"
: >f3 || pc_fail "cannot create f3"
: >f4 || pc_fail "cannot create f4"
: >f5 || pc_fail "cannot create f5"
pc_test
touch -d 2001-02-03T04:05:00Z f1 || pc_fail "touch -d with Z failed"
set -- $(ls -l f1)
[ "$6 $7 $8" = "Feb 3 2001" ] || pc_fail "-d ...Z gave $6 $7 $8"
touch -d 2001-02-03T04:06:00Z f2 || pc_fail "touch -d failed"
[ f2 -nt f1 ] || pc_fail "the minutes were ignored"
touch -d "2001-02-03 04:05:00" f3 || pc_fail "touch -d with a space failed"
[ ! f3 -nt f1 ] && [ ! f1 -nt f3 ] || pc_fail "T and space, or Z and the local time in TZ=UTC, differ"
touch -d 2001-02-03T04:05:06.5Z f4 || pc_fail "touch -d with a fraction failed"
touch -d 2001-02-03T04:05:06,5Z f5 || pc_fail "touch -d with a comma fraction failed"
touch -t 200102030405.06 f2 || pc_fail "touch -t failed"
touch -d 2001-02-03T04:05:06Z f3 || pc_fail "touch -d failed"
[ ! f3 -nt f2 ] && [ ! f2 -nt f3 ] || pc_fail "-t and -d give different times for the same instant"
pc_pass
