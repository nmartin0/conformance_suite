# ulimit -t writes one limit value.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.ulimit.007
. "$PC_CORE/pc.sh"
pc_begin XCU.ulimit.007
nl='
'
pc_test
v=$(ulimit -t 2>/dev/null)
rc=$?
[ "$rc" -eq 0 ] || pc_fail "ulimit -t exited $rc"
case $v in
*"$nl"*) pc_fail "ulimit -t wrote more than one line" ;;
unlimited) ;;
''|*[!0-9]*) pc_fail "ulimit -t wrote '$v', neither digits nor unlimited" ;;
esac
pc_pass
