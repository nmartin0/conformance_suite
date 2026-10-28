# iconv -l.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.iconv.001
. "$PC_CORE/pc.sh"
pc_begin XCU.iconv.001
LC_ALL=C; export LC_ALL
pc_test
iconv -l >out 2>err </dev/null
rc=$?
[ "$rc" -eq 0 ] || pc_fail "iconv -l exited with status $rc"
[ -s out ] || pc_fail "iconv -l wrote nothing to standard output"
pc_pass
