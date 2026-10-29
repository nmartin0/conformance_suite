# Message lines from standard input.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.logger.003
. "$PC_CORE/pc.sh"
pc_begin XCU.logger.003
LC_ALL=C; export LC_ALL
printf 'posixcheck line one\nposixcheck line two\n' >in
pc_test
# the file is shared with a read that follows: logger must have taken the
# message lines, so nothing is left for it
left=
{
	logger -t posixcheck >out 2>err
	rc=$?
	IFS= read -r left
} <in
[ "$rc" -eq 0 ] || pc_fail "logger reading standard input exited with status $rc"
[ -s out ] && pc_fail "logger wrote to standard output"
[ -z "$left" ] || pc_fail "logger left the message line '$left' unread on standard input"
pc_pass
