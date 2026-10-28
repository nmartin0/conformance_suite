# An operand that is neither category nor keyword.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.locale.007
. "$PC_CORE/pc.sh"
pc_begin XCU.locale.007
LC_ALL=C; export LC_ALL
pc_test
locale pc_no_such_keyword_9z >out 2>err </dev/null
rc=$?
[ "$rc" -gt 0 ] || pc_fail "locale of an unknown name exited with status 0"
pc_pass
