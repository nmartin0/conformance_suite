# If one source cannot be linked, ln diagnoses it, links the others, and exits non-zero.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.ln.013
. "$PC_CORE/pc.sh"
pc_begin XCU.ln.013
printf '1\n' >a || pc_fail "cannot create a"
printf '2\n' >b || pc_fail "cannot create b"
mkdir d || pc_fail "cannot create d"
pc_test
ln a nonexistent b d >out 2>err </dev/null
rc=$?
[ "$rc" -ne 0 ] || pc_fail "ln exited 0 although a source does not exist"
[ -s err ] || pc_fail "no diagnostic"
[ d/a -ef a ] || pc_fail "d/a was not made"
[ d/b -ef b ] || pc_fail "d/b was not made"
pc_pass
