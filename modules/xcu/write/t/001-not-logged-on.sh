# write to a user that is not logged on.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.write.001
. "$PC_CORE/pc.sh"
pc_begin XCU.write.001
LC_ALL=C; export LC_ALL
pc_test
printf 'hello\n' | write pc_no_such_login_zz >/dev/null 2>&1
rc=$?
[ "$rc" -gt 0 ] || pc_fail "write to a user who is not logged on exited with status 0"
pc_pass
