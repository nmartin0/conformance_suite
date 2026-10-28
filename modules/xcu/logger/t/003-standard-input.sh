# Message lines from standard input.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.logger.003
. "$PC_CORE/pc.sh"
pc_begin XCU.logger.003
LC_ALL=C; export LC_ALL
printf 'posixcheck line one\n' >in
pc_test
logger -t posixcheck <in >out 2>err
rc=$?
[ "$rc" -eq 0 ] || pc_fail "logger reading standard input exited with status $rc"
[ -s out ] && pc_fail "logger wrote to standard output"
pc_pass
