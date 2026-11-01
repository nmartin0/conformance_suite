# If one directory cannot be created, mkdir diagnoses it, creates the others, and exits non-zero.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.mkdir.009
# MUTANT    mkdir.stops_on_error
# MUTANT    mkdir.status0_on_error
# MUTANT    mkdir.status_last_only
# MUTANT    mkdir.no_diag
# MUTANT    mkdir.rollback
# VARIANT   mkdir.status2
# VARIANT   mkdir.status255
. "$PC_CORE/pc.sh"
pc_begin XCU.mkdir.009
printf x >f || pc_fail "cannot create f"
pc_test
mkdir d1 nodir/x d2 >out 2>err </dev/null
rc=$?
[ "$rc" -ne 0 ] || pc_fail "mkdir exited 0 although nodir does not exist"
[ -s err ] || pc_fail "no diagnostic"
[ -d d1 ] && [ -d d2 ] || pc_fail "the other directories were not created"
mkdir d3 nodir/y >out 2>err </dev/null
rc=$?
[ "$rc" -ne 0 ] || pc_fail "mkdir exited 0 although the last operand failed"
[ -s err ] || pc_fail "no diagnostic for the last operand"
[ -d d3 ] || pc_fail "d3 was not created"
mkdir -p f/x d4 >out 2>err </dev/null
rc=$?
[ "$rc" -ne 0 ] || pc_fail "mkdir -p exited 0 although f/x cannot be made"
[ -s err ] || pc_fail "no diagnostic with -p"
[ -d d4 ] || pc_fail "d4 was not created after the -p failure"
pc_pass
