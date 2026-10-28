# If one operand cannot be removed, rmdir diagnoses it, handles the others, and exits non-zero.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.rmdir.006
. "$PC_CORE/pc.sh"
pc_begin XCU.rmdir.006
mkdir d1 d2 || pc_fail "cannot create directories"
pc_test
rmdir d1 nonexistent d2 >out 2>err </dev/null
rc=$?
[ "$rc" -ne 0 ] || pc_fail "rmdir exited 0 although an operand does not exist"
[ -s err ] || pc_fail "no diagnostic"
[ ! -e d1 ] && [ ! -e d2 ] || pc_fail "the other directories were not removed"
pc_pass
