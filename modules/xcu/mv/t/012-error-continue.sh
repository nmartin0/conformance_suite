# If one source cannot be moved, mv diagnoses it, moves the remaining sources, and exits non-zero.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.mv.012
. "$PC_CORE/pc.sh"
pc_begin XCU.mv.012
printf 'A\n' >a || pc_fail "cannot create a"
printf 'B\n' >b || pc_fail "cannot create b"
mkdir d || pc_fail "cannot create d"
pc_test
mv a nonexistent b d >out 2>err </dev/null
rc=$?
[ "$rc" -ne 0 ] || pc_fail "mv exited 0 although a source does not exist"
[ -s err ] || pc_fail "no diagnostic"
[ -f d/a ] || pc_fail "d/a not moved"
[ -f d/b ] || pc_fail "d/b not moved"
pc_pass
