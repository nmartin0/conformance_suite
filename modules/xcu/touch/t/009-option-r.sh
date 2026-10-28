# touch -r copies both the modification and the access time of the reference file.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.touch.009
. "$PC_CORE/pc.sh"
pc_begin XCU.touch.009
TZ=UTC0; export TZ
touch -t 200101021504 ref || pc_fail "cannot create ref"
touch -a -t 200205060708 ref || pc_fail "cannot set the access time of ref"
: >f || pc_fail "cannot create f"
pc_test
touch -r ref f >out 2>err </dev/null
rc=$?
[ "$rc" -eq 0 ] || pc_fail "touch -r exited with status $rc"
set -- $(ls -l f)
[ "$6 $7 $8" = "Jan 2 2001" ] || pc_fail "modification time is $6 $7 $8"
set -- $(ls -lu f)
[ "$6 $7 $8" = "May 6 2002" ] || pc_fail "access time is $6 $7 $8"
pc_pass
