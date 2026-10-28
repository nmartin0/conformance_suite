# id with a user that does not exist.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.id.008
. "$PC_CORE/pc.sh"
pc_begin XCU.id.008
LC_ALL=C; export LC_ALL
pc_test
id pcnouser9zq >out 2>err </dev/null
rc=$?
[ "$rc" -gt 0 ] || pc_fail "id of a nonexistent user exited with status 0"
pc_pass
