# If one source cannot be linked, ln diagnoses it, links the others, and exits non-zero.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.ln.013
# MUTANT    ln.cont_stops
# MUTANT    ln.cont_status_last
# MUTANT    ln.err_silent
# MUTANT    ln.err_status0
# MUTANT    ln.exists_overwrite
# VARIANT   ln.status2
# VARIANT   ln.status255
. "$PC_CORE/pc.sh"
pc_begin XCU.ln.013
printf '1\n' >a || pc_fail "cannot create a"
printf '2\n' >b || pc_fail "cannot create b"
mkdir d d2 d3 || pc_fail "cannot create directories"
printf 'x\n' >d2/a || pc_fail "cannot create d2/a"
pc_test
ln a nonexistent b d >out 2>err </dev/null
rc=$?
[ "$rc" -ne 0 ] || pc_fail "ln exited 0 although a source does not exist"
[ -s err ] || pc_fail "no diagnostic"
[ d/a -ef a ] || pc_fail "d/a was not made"
[ d/b -ef b ] || pc_fail "d/b was not made"
ln a b d2 >out 2>err </dev/null
rc=$?
[ "$rc" -ne 0 ] || pc_fail "ln exited 0 although d2/a exists"
[ -s err ] || pc_fail "no diagnostic for the existing destination"
[ d2/b -ef b ] || pc_fail "d2/b was not made after d2/a failed"
[ ! d2/a -ef a ] || pc_fail "d2/a was replaced without -f"
ln nonexistent a d3 >out 2>err </dev/null
rc=$?
[ "$rc" -ne 0 ] || pc_fail "ln exited 0 although the first source does not exist"
[ -s err ] || pc_fail "no diagnostic for the first source"
[ d3/a -ef a ] || pc_fail "d3/a was not made after the first source failed"
pc_pass
