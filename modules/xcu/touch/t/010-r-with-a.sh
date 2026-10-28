# touch -r combined with -a changes only the access time, taking it from the reference file.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.touch.010
. "$PC_CORE/pc.sh"
pc_begin XCU.touch.010
TZ=UTC0; export TZ
touch -t 200101021504 ref || pc_fail "cannot create ref"
touch -a -t 200205060708 ref || pc_fail "cannot set the access time of ref"
touch -t 200301010101 f || pc_fail "cannot create f"
pc_test
touch -a -r ref f >out 2>err </dev/null
rc=$?
[ "$rc" -eq 0 ] || pc_fail "touch -a -r exited with status $rc"
set -- $(ls -lu f)
[ "$6 $7 $8" = "May 6 2002" ] || pc_fail "access time is $6 $7 $8"
set -- $(ls -l f)
[ "$6 $7 $8" = "Jan 1 2003" ] || pc_fail "modification time changed to $6 $7 $8"
pc_pass
