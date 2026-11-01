# touch -r with a reference file that does not exist fails with a diagnostic and a non-zero status.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.touch.011
# MUTANT    touch.err_no_diag
# MUTANT    touch.err_stdout
# MUTANT    touch.ref_status0
# MUTANT    touch.ref_ignored
# VARIANT   touch.err_status_3
. "$PC_CORE/pc.sh"
pc_begin XCU.touch.011
[ ! -e nonexistent ] || pc_fail "nonexistent exists"
: >f || pc_fail "cannot create f"
pc_test
touch -r nonexistent f >out 2>err </dev/null
rc=$?
[ "$rc" -ne 0 ] || pc_fail "touch exited 0 for a missing reference file"
[ -s err ] || pc_fail "no diagnostic"
[ ! -s out ] || pc_fail "the diagnostic went to standard output"
pc_pass
