# touch -a changes the access time only.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.touch.005
. "$PC_CORE/pc.sh"
pc_begin XCU.touch.005
TZ=UTC0; export TZ
touch -t 200101021504 f || pc_fail "cannot create f"
pc_test
touch -a f >out 2>err </dev/null
rc=$?
[ "$rc" -eq 0 ] || pc_fail "touch -a exited with status $rc"
set -- $(ls -lu f)
case $8 in
[0-9][0-9]:[0-9][0-9]) ;;
*) pc_fail "access time was not updated: $6 $7 $8" ;;
esac
set -- $(ls -l f)
[ "$6 $7 $8" = "Jan 2 2001" ] || pc_fail "modification time changed to $6 $7 $8"
pc_pass
