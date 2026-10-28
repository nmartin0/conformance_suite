# -f FILE.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.logger.004
. "$PC_CORE/pc.sh"
pc_begin XCU.logger.004
LC_ALL=C; export LC_ALL
printf 'posixcheck from a file\n' >msgs
[ -e pclogger_missing.$$ ] && pc_fail "setup: the missing file exists"
pc_test
logger -t posixcheck -f msgs >out 2>err </dev/null
rc=$?
[ "$rc" -eq 0 ] || pc_fail "logger -f exited with status $rc"
[ -s out ] && pc_fail "logger -f wrote to standard output"
logger -t posixcheck -f pclogger_missing.$$ >out 2>err </dev/null
rc=$?
[ "$rc" -gt 0 ] || pc_fail "logger -f of a missing file exited with status 0"
pc_pass
