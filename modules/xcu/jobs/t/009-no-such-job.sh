# A job_id that names no job fails.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.jobs.009
. "$PC_CORE/pc.sh"
pc_begin XCU.jobs.009
set -m || pc_fail "set -m is not accepted"
pc_test
jobs '%99' >out 2>/dev/null
rc=$?
[ "$rc" -gt 0 ] || pc_fail "jobs %99 exited $rc"
[ ! -s out ] || pc_fail "jobs %99 wrote to standard output"
pc_pass
