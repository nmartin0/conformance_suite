# date +%Y: status 0, a year on stdout, nothing on stderr.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.date.008
. "$PC_CORE/pc.sh"
pc_begin XCU.date.008
LC_ALL=C; export LC_ALL
pc_test
date +%Y >out 2>err </dev/null
rc=$?
[ "$rc" -eq 0 ] || pc_fail "date +%Y exited with status $rc"
[ -s err ] && pc_fail "date +%Y wrote to standard error"
read -r y <out
case $y in
[0-9][0-9][0-9][0-9]) ;;
*) pc_fail "date +%Y wrote '$y'" ;;
esac
pc_pass
