# With no jobs jobs writes nothing and exits 0.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.jobs.010
. "$PC_CORE/pc.sh"
pc_begin XCU.jobs.010
set -m || pc_fail "set -m is not accepted"
pc_test
jobs >out 2>/dev/null
rc=$?
[ "$rc" -eq 0 ] || pc_fail "jobs exited $rc"
[ ! -s out ] || pc_fail "jobs wrote output though no job exists"
pc_pass
