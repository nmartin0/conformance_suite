# touch -t accepts a time up to the start of 2038.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.touch.013
. "$PC_CORE/pc.sh"
pc_begin XCU.touch.013
TZ=UTC0; export TZ
: >f || pc_fail "cannot create f"
pc_test
touch -t 203701011200 f >out 2>err </dev/null
rc=$?
[ "$rc" -eq 0 ] || pc_unresolved "touch -t 203701011200 exited with status $rc (the file system may not hold that time)"
set -- $(ls -l f)
[ "$6 $7 $8" = "Jan 1 2037" ] || pc_fail "time is $6 $7 $8"
pc_pass
