# If one directory cannot be created, mkdir diagnoses it, creates the others, and exits non-zero.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.mkdir.009
. "$PC_CORE/pc.sh"
pc_begin XCU.mkdir.009
pc_test
mkdir d1 nodir/x d2 >out 2>err </dev/null
rc=$?
[ "$rc" -ne 0 ] || pc_fail "mkdir exited 0 although nodir does not exist"
[ -s err ] || pc_fail "no diagnostic"
[ -d d1 ] && [ -d d2 ] || pc_fail "the other directories were not created"
pc_pass
