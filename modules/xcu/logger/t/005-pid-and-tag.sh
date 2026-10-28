# -i and -t.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.logger.005
. "$PC_CORE/pc.sh"
pc_begin XCU.logger.005
LC_ALL=C; export LC_ALL
pc_test
logger -i -t posixcheck "with the process ID" >out 2>err </dev/null
rc=$?
[ "$rc" -eq 0 ] || pc_fail "logger -i -t exited with status $rc"
[ -s out ] && pc_fail "logger -i wrote to standard output"
[ -s err ] && pc_fail "logger -i wrote to standard error"
pc_pass
