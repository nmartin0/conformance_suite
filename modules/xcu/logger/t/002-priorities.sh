# The required facilities and levels.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.logger.002
. "$PC_CORE/pc.sh"
pc_begin XCU.logger.002
LC_ALL=C; export LC_ALL
pc_test
# every facility once and every level once, to keep the number of messages down
for p in user.alert local0.crit local1.err local2.warning local3.notice \
    local4.info local5.debug local6.notice local7.notice; do
	logger -p "$p" -t posixcheck "priority test $p" >out 2>err </dev/null
	rc=$?
	[ "$rc" -eq 0 ] || pc_fail "logger -p $p exited with status $rc"
	[ -s out ] && pc_fail "logger -p $p wrote to standard output"
done
pc_pass
