# ipcrm with no options.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.ipcrm.001
. "$PC_CORE/pc.sh"
pc_begin XCU.ipcrm.001
LC_ALL=C; export LC_ALL
pc_test
ipcrm >out 2>err </dev/null
rc=$?
[ "$rc" -eq 0 ] || pc_fail "ipcrm exited with status $rc"
[ -s out ] && pc_fail "ipcrm wrote to standard output"
pc_pass
