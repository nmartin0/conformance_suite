# Without options touch sets both the modification and the access time of an existing file to the current time.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.touch.003
. "$PC_CORE/pc.sh"
pc_begin XCU.touch.003
TZ=UTC0; export TZ
touch -t 200101021504 f || pc_fail "cannot create f"
pc_test
touch f >out 2>err </dev/null
rc=$?
[ "$rc" -eq 0 ] || pc_fail "touch exited with status $rc"
set -- $(ls -l f)
case $8 in
[0-9][0-9]:[0-9][0-9]) ;;
*) pc_fail "modification time is not recent: $6 $7 $8" ;;
esac
set -- $(ls -lu f)
case $8 in
[0-9][0-9]:[0-9][0-9]) ;;
*) pc_fail "access time is not recent: $6 $7 $8" ;;
esac
pc_pass
