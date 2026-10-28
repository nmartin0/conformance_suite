# logger with string operands.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.logger.001
. "$PC_CORE/pc.sh"
pc_begin XCU.logger.001
LC_ALL=C; export LC_ALL
pc_test
logger -t posixcheck "first part" second >out 2>err </dev/null
rc=$?
[ "$rc" -eq 0 ] || pc_fail "logger exited with status $rc"
[ -s out ] && pc_fail "logger wrote to standard output"
[ -s err ] && pc_fail "logger wrote to standard error"
pc_pass
